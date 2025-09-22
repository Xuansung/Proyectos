// Forma de compilar el programa: g++ -std=c++23 -o docserver main.cc safemap.cc
// MODIFICACIÓN EN LA LÍNEA 152
//Librería
#include <cstring> // Uso del std::strerror()
#include <cerrno> // Uso del acceso a errno, que contiene el código de error de la última función.
#include <string>
#include <iostream>
#include <sstream> // Uso de cadenas como flujos, como std::ostringstream.
#include <vector>
#include <expected> // Uso de la clase std::expected para manejo de errores.
#include <fcntl.h> // Proporciona constantes y funciones para control de archivos como open().
#include <string_view> // Uso de la clase std::string_view trabajar con cadenas sin copiar datos.
#include <sys/stat.h> // Uso para la función fstat() para obtener información de archivos.
#include <unistd.h> // Acceso a las funciones POSIX estándar como close().
#include <sys/mman.h> // Proporciona funciones par amapear arcihvos en memoria como mmap() y munmap().
#include "safemap.h" // Cabecera de la clase SafeMap.

//Flag global de verbose
bool g_verbose = false;
// Errores
enum class parse_args_errors {
  missing_argument,
  unknown_option,
};
// Opciones del programa
struct program_options {
  bool help = false;
  bool verbose = false;
  std::string output_filename;
};

std::expected<program_options, parse_args_errors> parse_args(int argc, char* argv[]) {
  // vector que contiene todos los argumentos de la línea de comandos, excluyendo el nombre del programa.
  std::vector<std::string_view> args(argv + 1, argv + argc);
  program_options options;

  for (auto it = args.begin(), end = args.end(); it != end; ++it) {
    if (*it == "-h" || *it == "--help") {
    options.help = true;
    } else if (*it == "-v" || *it == "--verbose") {
        options.verbose = true;
    } else if ((*it).starts_with("-")) {
      return std::unexpected(parse_args_errors::unknown_option);
    } else {
      options.output_filename = std::string(*it);
    }
  }

  if (!options.help && options.output_filename.empty()) {
    return std::unexpected(parse_args_errors::missing_argument);
  }
  return options;
}

void send_response(std::string_view header, std::string_view body = {}) {
  std::cout << header << std::endl;
  std::cout << body << std::endl;
}

void Mostrar_Ayuda() {
  std::cout << "Modo de uso: ./docserver [-h | --help] [-v | --verbose] ARCHIVO" << std::endl;
  std::cout << "Este programa muestra el tamaño del ARCHIVO y luego su contenido." << std::endl;
  std::cout << "Content-Length: <TAMAÑO>" << std::endl;
  std::cout << "<contenido del archivo...>" << std::endl;
  std::cout << "[-h | --help] muestra la ayuda." << std::endl;
  std::cout << "[-v | --verbose] activa el modo detallado. En este modo, adicionalmente mensajes " << std::endl;
  std::cout << "informativos por la salida de error." << std::endl;
}

std::expected<SafeMap, int> read_all(const std::string& path) {
  if (g_verbose) {
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

  if (g_verbose) {
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

std::string_view sv(reinterpret_cast<const char*>(mem), length);

SafeMap smap(sv);

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

g_verbose = options.value().verbose;

if (options.value().output_filename.empty()) {
  std::cerr << "Error: archivo no especificado." << std::endl;
  return 1;
}

auto lee_resultado = read_all(options.value().output_filename);
if (!lee_resultado.has_value()) {
  int err = lee_resultado.error();
  if (err == EACCES) {
    send_response("403 Forbidden\n");
    return 1;
  } else if (err == ENOENT) {
    send_response("404 Not Found\n");
    return 1;
  } else {
    std::cerr << "Error (" << err << "): " << std::strerror(err) << std::endl;
    return 1;
  }
}
SafeMap& smap = lee_resultado.value();
std::string_view content = smap.getSV();
std::ostringstream oss;
oss << "Content-Length: " << content.size() << std::endl;
std::string header = oss.str();

send_response(header, content);

return 0;
}


