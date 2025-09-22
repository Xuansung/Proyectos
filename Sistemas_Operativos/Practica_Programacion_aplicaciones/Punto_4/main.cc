// Forma de compilar el programa: g++ -std=c++23 -o docserver main.cc docserver.cc safefd.cc safemap.cc
// Librería
#include "docserver.h"

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
      int env_port_num = std::stoi(env_port);
      if (env_port_num >= 1 && env_port_num <= 65535) {
        port = static_cast<uint16_t>(env_port_num);
      } else {
        std::cerr << "Error: Puerto en DOCSERVER_PORT inválido. Usando puerto por defecto 8080." << std::endl;
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
  if (!resultado_socket.has_value()) {
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
    // Obtener info del cliente
    char cliente_ip[INET_ADDRSTRLEN];
    // Convierte dir_IP a una cadena legible
    inet_ntop(AF_INET, &dir_cliente.sin_addr, cliente_ip, sizeof(cliente_ip));
    uint16_t cliente_puerto = ntohs(dir_cliente.sin_port);
    // Ver si es un bin a ejecutar
    bool es_programa = false;
    if (ruta_archivo.starts_with("/bin/")) {
      es_programa = true;
    }

    if (es_programa) {
      // Preparar entorno
      exec_environment env;
      env.request_path = ruta_archivo;
      env.server_basedir = base_dir;
      env.remote_port = std::to_string(cliente_puerto);
      env.remote_ip = cliente_ip;
      std::string ruta_completa = base_dir + ruta_archivo;
      auto res_exec = execute_program(ruta_completa, env, verbose);
      if (!res_exec.has_value()) {
        // Error al ejecutar
        auto epe = res_exec.error();
        if (epe.error_code == ENOENT) {
          // Programa no existe
          send_response(cliente_socket, "404 Not Found", {}, verbose);
        } else if (epe.error_code == EACCES) {
          // Sin permisos
          send_response(cliente_socket, "403 Forbidden", {}, verbose);
        } else if (epe.error_code == ESUCCESS) {
          // Significa que se ejecutó, pero exit_code != 0
          send_response(cliente_socket, "500 Internal Server Error", {}, verbose);
        } else {
          // Otro error
          std::cerr << "Error al ejecutar el programa (" << epe.error_code << "): " << std::strerror(epe.error_code) << std::endl;
          send_response(cliente_socket, "500 Internal Server Error", {}, verbose);
        }
        continue;
      }
      // Éxito en la ejecución del programa
      std::string output = res_exec.value();
      std::ostringstream oss;
      oss << "Content-Length: " << output.size();
      std::string header = oss.str();
      int send_err = send_response(cliente_socket, header, output, verbose);
      if (send_err != 0) {
        if (send_err == ECONNRESET) {
          std::cerr << "Conexión cerrada por el cliente antes de recibir la respuesta" << std::endl;
        } else {
          std::cerr << "Error al enviar respuesta: " << std::strerror(send_err) << std::endl;
          return 1;
        }
      }
      continue;
    } else { 
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
}


