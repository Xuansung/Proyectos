// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 5: Construcción de un Autómata Finito
// Autor: Xuan Sun
// Correo: alu010175411@ull.edu.es
// Fecha: 22/10/2024
// Archivo automata_finito.cc
// Contiene las funciones de la clase automata_finito.cc.
// Funciones de la clase Automata_finito.
// Referencias:
// Enlaces de interés
// Historial de revisiones
// 20/10/2024 - Creación (primera versión) del código.
// 22/10/2024 - Última modificación.

//Librerías
#include "automata_finito.h"

/// @brief Constructor de la clase
AutomataFinito::AutomataFinito() : estado_arranque_(-1, false){
  num_estados_ = 0;
}

/// @brief Define el estado de arranque del AF
/// @param estado_arranque 
void AutomataFinito::setEstadoArranque(const Estado& estado_arranque) {
  estado_arranque_ = estado_arranque;
  estados_.insert(estado_arranque);
}

/// @brief Añade estados al AF
/// @param estado 
void AutomataFinito::AnadirEstados(const Estado& estado) {
  estados_.insert(estado);
  if (estado.getAceptado()) {
    estados_aceptados_.insert(estado);
  }
}

/// @brief Añade las transiciones del AF
/// @param estado_origen 
/// @param estado_destino 
/// @param simbolo 
void AutomataFinito::AnadirTransiciones(const Estado& estado_origen, 
  const Estado& estado_destino, char simbolo) {
  if (alfabeto_.find(simbolo) == alfabeto_.end()) {
    std::cout << "El simbolo no pertenece al alfabeto \n";
    std::exit(EXIT_FAILURE);
  }
  estados_.insert(estado_origen);
  estados_.insert(estado_destino);
  transiciones_.insert({{estado_origen, simbolo}, estado_destino});
}

/// @brief Lee los archivos de entrada
/// @param nombre_archivo 
/// @return true or false si se ha podido leer
bool AutomataFinito::LeerArchivo(const std::string& nombre_archivo){
  std::ifstream archivo(nombre_archivo);
  if (!archivo.is_open()) {
    std::cerr << "No se pudo abrir el archivo";
    return false;
  }
  std::string linea;
  int num_linea = 1;
  while (getline(archivo,linea)) {
    // Leer el alfabeto
    if (num_linea == 1) {
      for (char simbolo : linea) {
        if (simbolo != ' ') {
          // Si existe el símbolo '&' en el alfabeto devuelve false.
          if (simbolo == '&') {
            std::cerr << "Este símbolo no puede pertenecer a un alfabeto \n ";
            return false;
          }
          alfabeto_.insert(simbolo);
        }
      }
    }
  // Leer el número de estados.  
  if (num_linea == 2) {
    num_estados_ = std::stoi(linea);
  }
    // Leer cuál es el estado de arranque
  if (num_linea == 3) {
    int id_estado_arranque = std::stoi(linea);
    Estado estado_arranque(id_estado_arranque,false);
    setEstadoArranque(estado_arranque);
  }
  // Leer los estados y transiciones
  if (num_linea > 3 && num_linea <= 3 + num_estados_) {
    std::istringstream linea_estados(linea);
    int id_estado, aceptado, num_transiciones;
    if (!(linea_estados >> id_estado >> aceptado >> num_transiciones)) {
      std::cerr << "Error leyendo la definición del estado. \n";
      return false;
    }
    if (id_estado < 0 || id_estado >= num_estados_) {
      std::cerr << "El estado " << id_estado << " no es válido. \n";
      return false;
    }
    bool es_aceptado = (aceptado == 1);
    Estado estado(id_estado, es_aceptado);
    if(estado.getAceptado())
    estados_aceptados_.insert(estado);
    char simbolo_transicion;
    int id_estado_destino;
    for (int i = 0; i < num_transiciones; i++) {

      if (!(linea_estados >> simbolo_transicion >> id_estado_destino)) {
        std::cerr << "Error leyendo las transiciones del estado " << id_estado << ". \n";
        return false;
      }
      if (id_estado_destino < 0 || id_estado_destino >= num_estados_) {
        std::cerr << "El estado destino " << id_estado_destino << " no es válido. \n";
        return false;
      }
    }
    Estado estado_destino(id_estado_destino, false);
    AnadirTransiciones(estado, estado_destino, simbolo_transicion);
    AnadirEstados(estado);
  }
  num_linea++;
  }
  archivo.close();
  return true;
}

/// @brief Simula las cadenas de entrada para comprobar si es aceptado por el Autómata finito
/// @param cadena 
/// @return true false
bool AutomataFinito::SimularCadenas(const std::string& cadena) {
    Estado estado_actual = estado_arranque_;

  for (char simbolo : cadena) {
    if (alfabeto_.find(simbolo) == alfabeto_.end()) {
      return false;
    }
    std::pair<Estado, char> clave(estado_actual, simbolo);
    auto iteracion = transiciones_.find(clave); 
    if (iteracion == transiciones_.end()) {
      return false;
    }
    estado_actual = iteracion->second;
  }
  if (estados_aceptados_.find(estado_actual) != estados_aceptados_.end()) {
    return true;
  } else {
    return false;
  }
}