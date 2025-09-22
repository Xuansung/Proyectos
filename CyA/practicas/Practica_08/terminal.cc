// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 8: Gramáticas en Forma Normal de Chomsky.
// Autor: Xuan Sun
// Correo: alu010175411@ull.edu.es
// Fecha: 05/11/2024
// Archivo terminal.cc
// Contiene las funciones de la clase terminal.
// Funciones de la clase terminal.
// Referencias:
// Enlaces de interés
// Historial de revisiones
// 01/11/2024 - Creación (primera versión) del código.
// 05/11/2024 - Última modificación.

//Librerías
#include "terminal.h"

//Constructor de la clase
Terminal::Terminal (const char& valor) : valor_(valor) {}

/// @brief Devuelve el valor de la terminal
/// @return valor_
char Terminal::getValor() const {
  return valor_;
}

/// @brief Dice si es una terminal o no
/// @return true si es terminal.
bool Terminal::esTerminal() const {
  return true;
}