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
#include "noterminal.h"

//Constructor
NoTerminal::NoTerminal(const std::string& valor) : valor_(valor) {}

/// @brief Devuelve el valor del No Terminal
/// @return valor_
const std::string& NoTerminal::getValor() const {
  return valor_;
}
/// @brief Devuelve un false ya que no es un terminal
/// @return false
bool NoTerminal::EsTerminal() const {
  return false;
}

