// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 8: Gramáticas en Forma Normal de Chomsky.
// Autor: Xuan Sun
// Correo: alu010175411@ull.edu.es
// Fecha: 05/11/2024
// Archivo produccion.cc
// Contiene las funciones de la clase produccion.
// Funciones de la clase produccion.
// Referencias:
// Enlaces de interés
// Historial de revisiones
// 01/11/2024 - Creación (primera versión) del código.
// 05/11/2024 - Última modificación.

//Librería
#include "produccion.h"
//Constructor
Produccion::Produccion(const NoTerminal& izquierdo) : izquierdo_(izquierdo) {}

/// @brief Devuelve la parte izquierda de la producción
/// @return izquierdo_
const NoTerminal Produccion::getIzquierdo() const {
  return izquierdo_;
}
/// @brief Añade simbolos terminales
/// @param simbolo 
void Produccion::AnadirSimboloDerecho(const Terminal& simbolo) {
  terminal_derecho_.push_back(simbolo);
}
/// @brief Añade símbolos no terminales
/// @param simbolo 
void Produccion::AnadirSimboloDerecho(const NoTerminal& simbolo) {
  no_terminal_derecho_.push_back(simbolo);
}

/// @brief Devuelve el tamaño de la parte derecha de la producción
/// @return Tamaño de la parte derecha
int Produccion::TamanoDerecho() const {
  return terminal_derecho_.size() + no_terminal_derecho_.size();
}

/// @brief Devuelve true si el tamaño es = a 2.
/// @return true o false
bool Produccion::EsBinario() const {
  return TamanoDerecho() == 2;
}

/// @brief Devbuelve true si solo hay una terminal
/// @return  true o false
bool Produccion::EsTerminalSola() const {
  return terminal_derecho_.size() == 1 && no_terminal_derecho_.empty();
}

/// @brief devuelve el terminal derecho
/// @return terminal_derecho_
const std::vector<Terminal>& Produccion::GetTerminalDerecho() const {
  return terminal_derecho_;
}
/// @brief devuelve el no terminal derecho
/// @return no_terminal_derecho_
const std::vector<NoTerminal>& Produccion::GetNoTerminalDerecho() const {
  return no_terminal_derecho_;
}