// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 4: Expresiones regulares
// Autor: Xuan Sun
// Correo: alu010175411@ull.edu.es
// Fecha: 09/10/2024
// Archivo case.cc
// Contiene los métodos de la clase case.cc.
// Funciones de la clase case.
// Referencias:
// Enlaces de interés
// Historial de revisiones
// 05/10/2024 - Creación (primera versión) del código.
// 07/10/2024 - Revisión del código y añadir detalles y comentarios.

//Librerías usadas.
#include "case.h"

//Constructor
Case::Case(const std::string& valor, const int numero_linea) : valor_(valor), numero_linea_(numero_linea) {}

/// @brief Devuelve el valor de case
/// @return valor
std::string Case::getValor() const {
  return valor_;
}

/// @brief Devuelve el numero de línea que existe un case
/// @return  numero de línea
int Case::getNumero_linea() const{
  return numero_linea_;
}