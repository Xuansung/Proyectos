// Forma de compilar el programa: g++ -std=c++23 -o docserver main.cc docserver.cc safefd.cc safemap.cc
//Librería
#include "docserver.h"

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
  // recorre todos los argumentos
  for (auto i = args.begin(), end = args.end(); i != end; i++) {
    // Verifica si es un puerto el argumento leído
    if (next_is_port) {
      int port = std::stoi(std::string(*i));
      if (port < 1 || port > 65535) {
        return std::unexpected(parse_args_errors::unknown_option);
      }
      options.port = static_cast<uint16_t>(port);
      next_is_port = false;
      // Verifica si es un directorio base el argumento leído
    } else if (next_is_base) {
      options.dir_base = std::string(*i);
      next_is_base = false;
      // Verifica el resto de las opciones
    } else if (*i == "-h" || *i == "--help") {
      options.help = true;
    } else if (*i == "-v" || *i == "--verbose") {
      options.verbose = true;
    } else if (*i == "-p" || *i == "--port") {
      next_is_port = true;
    } else if (*i == "-b" || *i == "--base") {
      next_is_base = true;
    } else {
      // Si no coincide con ninguna devuelve error
      return std::unexpected(parse_args_errors::unknown_option);
    }
  }
  // Si quedó algún flag activado significa que faltó algún valor para la opción -p o -b
  if (next_is_port || next_is_base) {
    return std::unexpected(parse_args_errors::missing_argument);
  }
  return options;
}
 // Muestra la ayuda
void Mostrar_Ayuda() {
  std::cout << "Modo de uso: ./docserver [-h | --help] [-v | --verbose] [-p PORT | --port PORT] [ARCHIVO]" << std::endl;
  std::cout << "Este programa muestra el tamaño del ARCHIVO y luego su contenido. Y si es un programa lo ejecuta." << std::endl;
  std::cout << "Content-Length: <TAMAÑO>" << std::endl;
  std::cout << "<contenido del archivo...>" << std::endl;
  std::cout << "[-h | --help] muestra la ayuda." << std::endl;
  std::cout << "[-p PORT | --port PORT] especifica el puerto en el que el servidor escuchará conexiones." << std::endl;
  std::cout << "[-b BASE | --base BASE] Establece el directorio base donde buscar archivos solicitados por los clientes." << std::endl;
  std::cout << "[-v | --verbose] activa el modo detallado. En este modo, adicionalmente mensajes " << std::endl;
  std::cout << "informativos por la salida de error." << std::endl;
}
// Crea el socket
std::expected<SafeFD, int> make_socket(uint16_t port, bool verbose) {
  if (verbose) {
    std::cerr << "socket: creando socket." << std::endl;
  }
  // Llama a la función socket para crear un nuevo socket
  // AF_INET: Indica que el socket usará el protocolo IPv4.
  // SOCK_STREAM: socket tipo TCP
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    return std::unexpected(errno);
  }
  int opt = 1;
  // Llama a setsockopt para configurar opciones del socket
  // SOL_SOCKET: indica que se está configurando una opción a nivel de socket
  // SO_REUSEADDR: permite reutlizar direcciones locales inmediatamente después de cerrar el socket
  // &opt: puntero a la opción que se va a establecer(1 para activar)
  // sizeof(opt): tamaño de la opción
  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  // Declara e inicializa una estructura sockaddr_in con ceros.
  sockaddr_in local_address{};
  // Especifica que usará direcciones IPv4
  local_address.sin_family = AF_INET;
  // BIG ENDIAN
  local_address.sin_port = htons(port);
  // Configura la dirección IP para INADDR_ANY, lo que indica que el socket escuchará en todas las interfaces de red disponibles
  local_address.sin_addr.s_addr = INADDR_ANY;
  // Si falla el bind
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
// Verificando la escucha
int listen_connection(const SafeFD& socket, bool verbose) {
  if (verbose) {
    std::cerr << "escuchando: poniendo el socket a la escucha." << std::endl;
  }
  // Si falla la escucha
  if (listen(socket.getFD(), SOMAXCONN) < 0) {
    return errno;
  }
  return 0;
}
// Acepta la conexión
std::expected<SafeFD, int> accept_connection(const SafeFD& socket, sockaddr_in& dir_cliente, bool verbose) {
  socklen_t tam_direccion_cliente = sizeof(dir_cliente);
  // Conexión con el cliente
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
// Recibe la petición
std::expected<std::string, int> receive_request(const SafeFD& socket, size_t max_size, bool verbose) {
  // Se crea buffer de tamaño máximo inicializado con 0
  std::string buffer(max_size, 0);
  // Recibe los datos desde el socket
  int received = recv(socket.getFD(), buffer.data(), max_size, 0);
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
// Procesar petición
std::expected<std::string, int> process_request(const std::string& request, bool verbose) {

  if (request.empty()) {
    return std::unexpected(EINVAL);
  }
  // Analiza la petición
  std::istringstream iss(request);
  std::string metodo, ruta;
  iss >> metodo >> ruta;
  if (metodo != "GET" || ruta.empty()) { // Petición inválida
    return std::unexpected(EINVAL);
  }
  if (verbose) {
    std::cerr << "procesado: método " << metodo << ", ruta " << ruta << std::endl; 
  }
  return ruta;
}
// Enviar respuesta
int send_response(const SafeFD& socket, std::string_view header, std::string_view body , bool verbose) {
  // Construye la respuesta
  std::string response{header};
  response += "\n";
  if (!body.empty()) {
    response += "\n";
    response += body;
  }
  int total_sent = 0;
  // Enviar todos los bytes de respuesta
  while (total_sent < response.size()) {
    int sent = send(socket.getFD(), response.data() + total_sent, response.size() - total_sent, 0);
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
  // Se abre el archivo en modo lectura
  int fd = open(path.c_str(), O_RDONLY);
  if (fd < 0) {
    return std::unexpected(errno);
  }
  // fstat obtiene la información del archivo abierto
  struct stat st;
  if (fstat(fd,&st) != 0) {
    int err = errno;
    close(fd);
    return std::unexpected(err);
  }
  int length = st.st_size;
  if (verbose) {
    std::cerr << "mmap: se mapea el archivo /" << path << "/ en memoria" << std::endl;
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
//Ejecución de programas
std::expected<std::string, execute_program_error> execute_program(const std::string& path, const exec_environment& env, bool verbose) {
  // Verificar que el programa existe y es ejecutable
  if (access(path.c_str(), X_OK) != 0) {
    execute_program_error epe;
    epe.exit_code = 0;
    epe.error_code = errno; // ENOENT, EACCES, etc.
    return std::unexpected(epe);
  }
  // Crear tubería
  int pipefd[2];
  // Si falla la comunicación entre padre e hijo
  if (pipe(pipefd) < 0) {
    execute_program_error epe;
    epe.exit_code = 0;
    epe.error_code = errno;
    return std::unexpected(epe);
  }
  // Duplica el proceso actual
  pid_t pid = fork();
  // Falla el fork
  if (pid < 0) {
    close(pipefd[0]);
    close(pipefd[1]);
    execute_program_error epe;
    epe.exit_code = 0;
    epe.error_code = errno;
    return std::unexpected(epe);
  }
  // Devuelve el proceso hijo
  if (pid == 0) {
    // Proceso hijo
    // Redirigir la salida estandar del programa hijo al extremo de escritura del pipe.
    dup2(pipefd[1], STDOUT_FILENO);
    // Se cierra los extremos innecesarios del pipe para evitar fugas.
    close(pipefd[0]);
    close(pipefd[1]);
    // Configurar variables de entorno
    setenv("REQUEST_PATH", env.request_path.c_str(), 1);
    setenv("REQUEST_BASEDIR", env.server_basedir.c_str(), 1);
    setenv("REMOTE_PORT", env.remote_port.c_str(), 1);
    setenv("REMOTE_IP", env.remote_ip.c_str(), 1);
    // Preparar argumentos para ejecutar el programa
    const char* argv_exec[] = { path.c_str(), NULL};
    if (verbose) {
      std::cerr << "exec: ejecutando /" << path << "/" << std::endl;
    }
    execvp(path.c_str(), (char* const*)argv_exec);
    // Si llega aquí, exec falló
    int err = errno;
    std::cerr << "Error al ejecutar " << path << ": " << std::strerror(err) << std::endl;
    // Salir con código distinto de 0 para indicar error
    _exit(127);
  }
  // Proceso padre
  close(pipefd[1]);
  // Leer la salida del hijo
  std::string output;
  char buf[1024];
  int n;
  while ((n = read(pipefd[0], buf, sizeof(buf))) > 0) {
    output.append(buf, n);
  }
  // Si falla al leer terminal al hijo con kill y espera con waitpid
  if (n < 0) {
    int err = errno;
    close(pipefd[0]);
    kill(pid, SIGKILL);
    waitpid(pid, NULL, 0);
    execute_program_error epe;
    epe.exit_code = 0;
    epe.error_code = err;
    return std::unexpected(epe);
  }
  close(pipefd[0]);
  int status;
  // Espera a que el hijo termine
  if (waitpid(pid, &status, 0) < 0) {
    execute_program_error epe;
    epe.exit_code = 0;
    epe.error_code = errno;
    return std::unexpected(epe);
  }
  // Comprueba si el hijo terminó bien
  if (WIFEXITED(status)) {
    int code = WEXITSTATUS(status);
    if (code == 0) {
      // Ejecución correcta
      return output;
    } else {
      // El programa terminó con error
      execute_program_error epe;
      epe.exit_code = code;
      epe.error_code = ESUCCESS;
      return std::unexpected(epe);
    }
  } else {
    // El proceso no terminó normalmente
      execute_program_error epe;
      epe.exit_code = 0;
      epe.error_code = EFAULT; // Error genérico
      return std::unexpected(epe);
  }
}