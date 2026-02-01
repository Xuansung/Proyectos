// docserver.h
#ifndef DOCSERVER_H
#define DOCSERVER_H

#include <cstring> // Uso del std::strerror()
#include <cerrno> // Uso del acceso a errno, que contiene el código de error de la última función.
#include <string>
#include <iostream>
#include <sstream> // Uso de cadenas como flujos, como std::ostringstream.
#include <vector>
#include <system_error> // Manejo avanzado de errores std::error_code.
#include <netinet/in.h> // Definiciones para trabajar con sockets.
#include <arpa/inet.h> // Funciones para manejar direcciones IP.
#include <sys/socket.h> // Funciones de sockets.
#include <expected> // Uso de la clase std::expected para manejo de errores.
#include <fcntl.h> // Proporciona constantes y funciones para control de archivos como open().
#include <string_view> // Uso de la clase std::string_view trabajar con cadenas sin copiar datos.
#include <sys/stat.h> // Uso para la función fstat() para obtener información de archivos.
#include <unistd.h> // Acceso a las funciones POSIX estándar como close().
#include <sys/mman.h> // Proporciona funciones par amapear arcihvos en memoria como mmap() y munmap().
#include <signal.h> // Manejo de señales como SIGKILL.
#include <sys/wait.h> // Para manejar procesos hijos.
#include "safemap.h" // Cabecera de la clase SafeMap.
#include "safefd.h" // Cabecera de la clase SafeFD

// Errores para el paso de argumentos
enum class parse_args_errors {
  missing_argument,
  unknown_option,
};

// Opciones del programa
struct program_options {
  bool help = false;    // Mostrar ayuda
  bool verbose = false; // Mostrar más detalles
  std::string dir_base; // dirección base
  uint16_t port = 8080; // Puerto por defecto
};

// Estructura para errores al ejecutar programa
struct execute_program_error {
  int exit_code;  // Código de salida del programa hijo o 0 si no se llegó a ejecutar.
  int error_code; // ENOENT, EACCES, etc. o ESUCCESS (0) si se ejecutó.
};

// Código de "éxito" especial
const int ESUCCESS = 0;

// Estructura para las variables de entorno que se le pasan al proceso hijo.
struct exec_environment {
  std::string request_path;     // Ruta del archivo solicitado
  std::string server_basedir;   // Directorio base del servidor
  std::string remote_port;      // Puerto del cliente
  std::string remote_ip;        // Dirección IP del cliente
};

// Declaraciones de funciones
//Obtiene el valor de una variable de entorno
std::string get_env(const std::string& nombre);
//Analiza los argumentos de la línea de comando y devuelve las opciones del programa o un error
std::expected<program_options, parse_args_errors> parse_args(int argc, char* argv[]);
// Muestra la ayuda
void Mostrar_Ayuda();
//Crea un socket en el puerto especificado
std::expected<SafeFD, int> make_socket(uint16_t port, bool verbose);
// Pone el socket a la escucha para aceptar las conexiones
int listen_connection(const SafeFD& socket, bool verbose);
// Acepta una conexión entrante y devuelve el SafeFD del nuevo socket
std::expected<SafeFD, int> accept_connection(const SafeFD& socket, sockaddr_in& dir_cliente, bool verbose);
// Recibe la petición del cliente desde el socket, hasta max_size bytes
std::expected<std::string, int> receive_request(const SafeFD& socket, size_t max_size, bool verbose);
// Procesa la petición recibida
std::expected<std::string, int> process_request(const std::string& request, bool verbose);
// Envía una respuesta al cliente
int send_response(const SafeFD& socket, std::string_view header, std::string_view body = {}, bool verbose = false);
// Lee un archivo completo en memoria usando mmap() y devuelve un SafeMap que lo encapsula
std::expected<SafeMap, int> read_all(const std::string& path, bool verbose);
// Ejecuta el programa dado
std::expected<std::string, execute_program_error> execute_program(const std::string& path, const exec_environment& env, bool verbose);

#endif // DOCSERVER_H
