
// Forma de compilar el programa: g++ -std=c++23 -o docserver main.cc safefd.cc safemap.cc
//Librería
#include <cstring> // Uso del std::strerror()
#include <cerrno> // Uso del acceso a errno, que contiene el código de error de la última función.
#include <string>
#include <iostream>
#include <sstream> // Uso de cadenas como flujos, como std::ostringstream.
#include <vector>
#include <system_error>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <expected> // Uso de la clase std::expected para manejo de errores.
#include <fcntl.h> // Proporciona constantes y funciones para control de archivos como open().
#include <string_view> // Uso de la clase std::string_view trabajar con cadenas sin copiar datos.
#include <sys/stat.h> // Uso para la función fstat() para obtener información de archivos.
#include <unistd.h> // Acceso a las funciones POSIX estándar como close().
#include <sys/mman.h> // Proporciona funciones par amapear arcihvos en memoria como mmap() y munmap().
#include "safemap.h" // Cabecera de la clase SafeMap.
#include "safefd.h" // Cabecera de la clase SafeFD

// Errores
enum class parse_args_errors {
  missing_argument,
  unknown_option,
};
// Opciones del programa
struct program_options {
  bool help = false;
  bool verbose = false;
  std::string dir_base;
  uint16_t port = 8080; // Puerto por defecto
};
// Leer las variables de entorno
std::string get_env(const std::string& nombre) {
  char* valor = std::getenv(nombre.c_str());
  if (valor) {
    return std::string{valor};
  }
  return {};
}

std::expected<program_options, parse_args_errors> parse_args(int argc, char* argv[]) {
  // vector que contiene todos los argumentos de la línea de comandos, excluyendo el nombre del programa.
  std::vector<std::string_view> args(argv + 1, argv + argc);
  program_options options;
  bool next_is_port = false;
  bool next_is_base = false;
  for (auto i = args.begin(), end = args.end(); i != end; i++) {
    if (next_is_port) {
      try {
        int port = std::stoi(std::string(*i));
        if (port < 1 || port > 65535) {
          return std::unexpected(parse_args_errors::unknown_option);
        }
        options.port = static_cast<uint16_t>(port);
      } catch (...) {
        return std::unexpected(parse_args_errors::unknown_option);
      }
      next_is_port = false;
    } else if (next_is_base) {
      options.dir_base = std::string(*i);
      next_is_base = false;
    } else if (*i == "-h" || *i == "--help") {
      options.help = true;
    } else if (*i == "-v" || *i == "--verbose") {
      options.verbose = true;
    } else if (*i == "-p" || *i == "--port") {
      next_is_port = true;
    } else if (*i == "-b" || *i == "--base") {
      next_is_base = true;
    } else {
      return std::unexpected(parse_args_errors::unknown_option);
    }
  }
  if (next_is_port || next_is_base) {
    return std::unexpected(parse_args_errors::missing_argument);
  }
  return options;
}
 
void Mostrar_Ayuda() {
  std::cout << "Modo de uso: ./docserver [-h | --help] [-v | --verbose] [-p PORT | --port PORT] ARCHIVO" << std::endl;
  std::cout << "Este programa muestra el tamaño del ARCHIVO y luego su contenido." << std::endl;
  std::cout << "Content-Length: <TAMAÑO>" << std::endl;
  std::cout << "<contenido del archivo...>" << std::endl;
  std::cout << "[-h | --help] muestra la ayuda." << std::endl;
  std::cout << "[-p PORT | --port PORT] especifica el puerto en el que el servidor escuchará conexiones." << std::endl;
  std::cout << "[-b BASE | --base BASE] Establece el directorio base donde buscar archivos solicitados por los clientes." << std::endl;
  std::cout << "[-v | --verbose] activa el modo detallado. En este modo, adicionalmente mensajes " << std::endl;
  std::cout << "informativos por la salida de error." << std::endl;
}

std::expected<SafeFD, int> make_socket(uint16_t port, bool verbose) {
  if (verbose) {
    std::cerr << "socket: creando socket." << std::endl;
  }
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    return std::unexpected(errno);
  }
  int opt = 1;
  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  sockaddr_in local_address{};
  local_address.sin_family = AF_INET;
  local_address.sin_port = htons(port);
  local_address.sin_addr.s_addr = INADDR_ANY;
  if (bind(fd, reinterpret_cast<const sockaddr*>(&local_address), sizeof(local_address)) < 0) {
    int err = errno;
    close(fd);
    return std::unexpected(err);
  }
  if (verbose) {
    std::cerr << "bind: asignado al puerto " << port << std::endl;
  }
  return SafeFD{fd};
}

int listen_connection(const SafeFD& socket, bool verbose) {
  if (verbose) {
    std::cerr << "escuchando: poniendo el socket a la escucha." << std::endl;
  }
  if (listen(socket.getFD(), SOMAXCONN) < 0) {
    return errno;
  }
  return 0;
}

std::expected<SafeFD, int> accept_connection(const SafeFD& socket, sockaddr_in& dir_cliente, bool verbose) {
  socklen_t tam_direccion_cliente = sizeof(dir_cliente);
  int nuevo_fd = accept(socket.getFD(), reinterpret_cast<sockaddr*>(&dir_cliente),&tam_direccion_cliente);
  if (nuevo_fd < 0) {
    return std::unexpected(errno);
  }
  if (verbose) {
    char cliente_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &dir_cliente.sin_addr, cliente_ip, sizeof(cliente_ip));
    std::cerr << "aceptado: conexión aceptada desde " << cliente_ip << ":" << ntohs(dir_cliente.sin_port) << std::endl;
  }
  return SafeFD{nuevo_fd};
}

std::expected<std::string, int> receive_request(const SafeFD& socket, size_t max_size, bool verbose) {
  std::string buffer(max_size, 0);
  ssize_t received = recv(socket.getFD(), buffer.data(), max_size, 0);
  if (received < 0) {
    return std::unexpected(errno);
  } else if (received == 0) {
    return std::unexpected(ECONNRESET);
  }
  buffer.resize(received);
  if (verbose) {
    std::cerr << "recibido: petición de " << received << " bytes" << std::endl;
  }
  return buffer;
}

std::expected<std::string, int> process_request(const std::string& request, bool verbose) {
  if (request.empty()) {
    return std::unexpected(EINVAL);
  }
  std::istringstream iss(request);
  std::string metodo, ruta;
  iss >> metodo >> ruta;
  if (metodo != "GET" || ruta.empty()) { // Petición inválida
    return std::unexpected(EINVAL);
  }
  if(ruta[0] != '/') {  // Ruta inválida
    return std::unexpected(EINVAL);
  }
  if (verbose) {
    std::cerr << "procesado: método " << metodo << ", ruta " << ruta << std::endl; 
  }
  return ruta;
}

int send_response(const SafeFD& socket, std::string_view header, std::string_view body = {}, bool verbose = false) {
  std::string response{header};
  response += "\n";
  if (!body.empty()) {
    response += "\n";
    response += body;
  }
  size_t total_sent = 0;
  while (total_sent < response.size()) {
    ssize_t sent = send(socket.getFD(), response.data() + total_sent, response.size() - total_sent, 0);
    if (sent < 0) {
      return errno;
    }
    total_sent += sent;
  }

  if (verbose) {
    std::cerr << "enviado: respuesta enviada\n";
  }
  return 0;
}

std::expected<SafeMap, int> read_all(const std::string& path, bool verbose) {
  if (verbose) {
    std::cerr << "open: se abre el archivo /" << path << "/" << std::endl;
  }
  int fd = open(path.c_str(), O_RDONLY);
  if (fd < 0) {
    return std::unexpected(errno);
  }
  struct stat st;
  if (fstat(fd,&st) != 0) {
    int err = errno;
    close(fd);
    return std::unexpected(err);
  }
  int length = st.st_size;
  if (verbose) {
    std::cerr << "mmap: se mape el archivo /" << path << "/ en memoria" << std::endl;
  }
// Se mapea el archivo completo en memoria para solo lectura y de forma privada.
  void* mem = mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, 0);
  if (mem == MAP_FAILED) {
    int err = errno;
    close(fd);
    return std::unexpected(err);
  }
// Opcionalmente, se puede cerrar el descriptor del archivo si ya no se necesita.
close(fd);

SafeMap smap(mem, length);

return smap;
}

int main (const int argc, char* argv[]) {
  auto options = parse_args(argc, argv); 
if (!options.has_value()) {
// Usar options.error() para comprobar el motivo del error...
if (options.error() == parse_args_errors::missing_argument) {
// Mostrar mensaje de error por falta de argumento...
  std::cerr << "Error: argumento perdido" << std::endl;
} else if (options.error() == parse_args_errors::unknown_option) {
    // Mostrar mensaje de error por opción desconocida...
    std::cerr << "Error: opción desconocida" << std::endl;
  }
  return 1;
}
// Usar options-> para acceder a las opciones...
if (options->help) {
  Mostrar_Ayuda();
  return 0;
}
bool verbose = options->verbose;
uint16_t port = options->port;
if(port == 8080) {
  std::string env_port = get_env("DOCSERVER_PORT");
  if (!env_port.empty()) {
    try {
      int env_port_num = std::stoi(env_port);
      if (env_port_num >= 1 && env_port_num <= 65535) {
        port = static_cast<uint16_t>(env_port_num);
      } else {
        std::cerr << "Error: Puerto en DOCSERVER_PORT inválido. Usando puerto por defecto 8080." << std::endl;
      }
    } catch (...) {
        std::cerr << "Error: Puert en DOCSERVER_PORT inválido. Usando puerto por defecto 8080." << std::endl;
    }
  }
}
std::string base_dir;
if (!options->dir_base.empty()) {
  base_dir = options->dir_base;
} else {
  base_dir = get_env("DOCSERVER_BASEDIR");
  if (base_dir.empty()) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
      base_dir = cwd;
    } else {
      std::cerr << "Error: no se pudo obtener el directorio actual." << std::endl;
      return 1;
    }
  }
}
if(verbose) {
  std::cerr << "Directorio base establecido en: " << base_dir << std::endl;
}
auto resultado_socket = make_socket(port, verbose);
if(!resultado_socket.has_value()) {
  std::cerr << "Error al crear el socket: " << std::strerror(resultado_socket.error()) << std::endl;
  return 1;
}
SafeFD& server_socket = resultado_socket.value();

int listen_err = listen_connection(server_socket, verbose);
if (listen_err != 0) {
  std::cerr << "Error en listen: " << std::strerror(listen_err) << std::endl;
  return 1;
}

while (true) {
  sockaddr_in dir_cliente{};
  auto resultado_cliente = accept_connection(server_socket, dir_cliente, verbose);
  if(!resultado_cliente.has_value()) {
    std::cerr << "Error en accept: " << std::strerror(resultado_cliente.error()) << std::endl;
    continue;
  }
  SafeFD& cliente_socket = resultado_cliente.value();
  //Leer la petición del cliente
  auto resultado_peticion = receive_request(cliente_socket, 4096, verbose);
  if (!resultado_peticion.has_value()) {
    int err = resultado_peticion.error();
    if (err == ECONNRESET) {
      std::cerr << "Error: conexión cerrada por el cliente antes de enviar la petición. " << std::endl;
      continue;
    } else {
      std::cerr << "Error al leer la petición: " << std::strerror(err) << std::endl;
      return 1;
    }
  }
  std::string peticion = resultado_peticion.value();
  // Procesar la petición
  auto resultado_ruta = process_request(peticion, verbose);
  if (!resultado_ruta.has_value()) {
    send_response(cliente_socket, "400 Bad Request", {}, verbose);
    if (verbose) {
      std::cerr << "Petición inválida recibida." << std::endl;
    }
    continue;
  }
  std::string ruta_archivo = resultado_ruta.value();
  // Construir la ruta completa
  std::string ruta_completa = base_dir + ruta_archivo;
  // Leer el archivo solicitado
  auto resultado_archivo = read_all(ruta_completa, verbose);
  if (!resultado_archivo.has_value()) {
    int err = resultado_archivo.error();  
    std::string header;
    if (err == EACCES) {
      header = "403 Forbidden";
      send_response(cliente_socket, header, {}, verbose);
    } else if ( err == ENOENT) {
      header = "404 Not Found";
      send_response(cliente_socket, header, {}, verbose);
    } else {
      std::cerr << "Error al leer el archivo (" << err << "): " << std::strerror(err) << std::endl;
      send_response(cliente_socket, "500 Internal Server Error", {}, verbose);
      return 1;
    }
    continue;
  }
  SafeMap& smap = resultado_archivo.value();
  std::string_view content = smap.getSV();
  std::ostringstream oss;
  oss << "Content-Length: " << content.size();
  std::string header = oss.str();

  int send_err = send_response(cliente_socket, header, content, verbose);
  if (send_err != 0) {
    if (send_err == ECONNRESET) {
      std::cerr << "Conexión cerrada por el cliente antes de recibir la respuesta" << std::endl;
    } else {
        std::cerr << "Error al enviar respuesta: " << std::strerror(send_err) << std::endl;
        return 1;
    }
  } 
  // SafeFD se encargará de cerrar el socket del cliente al salir del ámbito
}
return 0;
}


