// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 8: Gramáticas en Forma Normal de Chomsky.
// Autor: Xuan Sun
// Correo: alu010175411@ull.edu.es
// Fecha: 05/11/2024
// Archivo gramatica.cc
// Contiene las funciones de la clase gramatica.cc.
// Funciones de la clase Gramatica.
// Referencias:
// Enlaces de interés
// Historial de revisiones
// 01/11/2024 - Creación (primera versión) del código.
// 05/11/2024  - Última modificación.

//Librerías

#include "gramatica.h"

//Constructor
Gramatica::Gramatica()  {}
/// @brief Lee el Archivo de entrada
/// @param archivo 
/// @return 1 si se puede leer, 0 si no se puede 
bool Gramatica::CargarArchivo(const std::string& archivo) {
  
  std::ifstream archivo_entrante(archivo);
  if (!archivo_entrante.is_open())
    return false;

  std::string linea;
  int num_simbolos;
//Lee los símbolos terminales
  std::getline(archivo_entrante, linea);
    num_simbolos = std::stoi(linea);
    for (int i = 0; i < num_simbolos; i++) {
      std::getline(archivo_entrante, linea);
      terminales_.push_back(linea[0]);
    }
//Lee los símbolos no terminales
  std::getline(archivo_entrante, linea);
    num_simbolos = std::stoi(linea);
    for (int i = 0; i < num_simbolos; i++) {
      std::getline(archivo_entrante, linea);
      if (i == 0) 
        simbolo_arranque_ = NoTerminal(linea);
      noterminales_.push_back(linea);
    }
//Lee las producciones
  std::getline(archivo_entrante, linea);
  int num_producciones = std::stoi(linea);
  for (int i = 0; i < num_producciones; i++) {
    std::getline(archivo_entrante, linea);
    std::istringstream iss(linea);
    std::string izquierdo, parte_derecho;
    iss >> izquierdo;
    NoTerminal izquierdo_noterminal(izquierdo);
    Produccion produccion(izquierdo_noterminal);

    //Leer el lado derecho de la producción
    while (iss >> parte_derecho) {
      if (parte_derecho.size() == 1 && EsTerminal(parte_derecho[0])) 
        produccion.AnadirSimboloDerecho(Terminal(parte_derecho[0]));
        else if (EsNoTerminal(parte_derecho))
        produccion.AnadirSimboloDerecho(NoTerminal(parte_derecho));
        else
        std::cerr << "Símbolo desconocido en la producción: " << parte_derecho << std::endl;
    }
    producciones_.push_back(produccion);
  }
  archivo_entrante.close();
  return true;
}

/// @brief Imprime el resultado en un archivo de salida.
/// @param archivo 
/// @return 1 si se logró, 0 si no se pudo.
bool Gramatica::GuardarArchivo(const std::string& archivo) {
  std::ofstream archivo_salida(archivo);
  if (!archivo_salida.is_open())
    return false;

  archivo_salida << producciones_.size() << std::endl;
  for (const auto& produccion : producciones_) {
    archivo_salida << produccion.getIzquierdo().getValor() << " ";
    for (const auto& terminal : produccion.GetTerminalDerecho()) {
      archivo_salida << terminal.getValor() << " ";
    }
    for (const auto& noterminal : produccion.GetNoTerminalDerecho()) {
      archivo_salida << noterminal.getValor() << " ";
    }
    archivo_salida << std::endl;
  }
  archivo_salida.close();
  return true;
}


/// @brief Convierte la gramática en Forma Normal de Chomsky
void Gramatica::ConvertirCNF() {
  ReemplazaTerminalProduccion();
  DivideProduccion();
}

/// @brief Verifica si tiene producciones vacías la gramática.
/// @return 1 si hay producciones vacías, 0 si no.
bool Gramatica::ProduccionesVacias() const {
  for (const auto& produccion : producciones_) {
    if (produccion.GetTerminalDerecho().empty() && produccion.GetNoTerminalDerecho().empty())
      return true;
  }
  return false;
}

/// @brief Verifica si tiene producciones unitarias la gramática.
/// @return 1 si hay producciones unitarias, 0 si no.
bool Gramatica::ProduccionesUnitarias() const {
    for (const auto& produccion : producciones_) {
    if (produccion.GetTerminalDerecho().empty() && produccion.GetNoTerminalDerecho().size() == 1)
      return true;
  }
  return false;
}

/// @brief Hace la primera parte del algoritmo de Forma Normal de Chomsky
void Gramatica::ReemplazaTerminalProduccion() {
  std::vector<Produccion> nuevas_producciones;
  for (auto& produccion : producciones_) {
    std::vector<Terminal> terminales = produccion.GetTerminalDerecho();
    if (terminales.size() >= 2) {
      for (auto& terminal : terminales) {
        std::string nuevo_noterminal = NuevoNoTerminal();
        noterminales_.push_back(nuevo_noterminal);
        NoTerminal auxiliar(nuevo_noterminal);
        Produccion nueva_produccion(auxiliar);
        nueva_produccion.AnadirSimboloDerecho(terminal);
        nuevas_producciones.push_back(nueva_produccion);
      }
    }
  }
  producciones_.insert(producciones_.end(),nuevas_producciones.begin(),nuevas_producciones.end());
}

/// @brief Hace la segunda parte del algoritmo de Forma Normal de Chomsky
void Gramatica::DivideProduccion() {
  std::vector<Produccion> producciones_actualizados;
  for (auto& produccion : producciones_) {
    if (produccion.TamanoDerecho() > 2) {
      std::string izquierdo_actual = produccion.getIzquierdo().getValor();
      for ( int i = 0; i < produccion.TamanoDerecho() - 2; i++) {
        std::string nuevo_noterminal = NuevoNoTerminal();
        noterminales_.push_back(nuevo_noterminal);
        NoTerminal auxiliar(izquierdo_actual);
        Produccion nueva_produccion(auxiliar);
        nueva_produccion.AnadirSimboloDerecho(NoTerminal(produccion.GetNoTerminalDerecho()[i].getValor()));
        nueva_produccion.AnadirSimboloDerecho(NoTerminal(nuevo_noterminal));
        producciones_actualizados.push_back(nueva_produccion);
        izquierdo_actual = nuevo_noterminal;
      }
      NoTerminal auxiliar(izquierdo_actual);
      Produccion produccion_final(auxiliar);
      produccion_final.AnadirSimboloDerecho(produccion.GetNoTerminalDerecho().back());
      producciones_actualizados.push_back(produccion_final);
    } else {
      producciones_actualizados.push_back(produccion);
    }
  }
  producciones_ = producciones_actualizados;
}

/// @brief Verifica si es terminal el símbolo dado
/// @param simbolo 
/// @return 1 si es terminal , 0 si no
bool Gramatica::EsTerminal(char simbolo) const {
  for (const char& terminal : terminales_) {
    if (terminal == simbolo) {
      return true;
    }
  }
  return false;
}


/// @brief Verifica si es no terminal el símbolo dado
/// @param simbolo 
/// @return 1 si no es terminal, 0 si no
bool Gramatica::EsNoTerminal( const std::string& simbolo) const {
  for (const std::string& noterminal : noterminales_) {
    if (noterminal == simbolo) {
      return true;
    }
  }
  return false;
}

/// @brief Genera un no terminal nuevo
/// @return no terminal nuevo
std::string Gramatica::NuevoNoTerminal() {
  return "C" + std::to_string(noterminales_.size());
}
