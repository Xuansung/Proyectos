// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 4: Expresiones regulares
// Autor: Xuan Sun
// Correo: alu010175411@ull.edu.es
// Fecha: 07/10/2024
// Archivo variable.cc
// Contiene los métodos de la clase variable.
// Métodos de la clase variable.
// Referencias:
// Enlaces de interés
// Historial de revisiones
// 05/10/2024 - Creación (primera versión) del código.
// 07/10/2024 - Revisión del código y añadir detalles y comentarios.

//Librería usada
#include "variable.h"

/// @brief Constructor de la clase Variable
/// @param tipo_variable 
/// @param nombre 
/// @param valor 
/// @param linea 
Variable::Variable(const std::string& tipo_variable, const std::string& nombre, const std::string& valor, const int linea)
: tipo_variable_(tipo_variable), nombre_(nombre), valor_(valor), linea_(linea) {}

/// @brief Devuelve el tipo de variable
/// @return tipo de variable
std::string Variable::getTipo_variable() const{
  return tipo_variable_;
}

/// @brief Devuelve el nombre de la variable
/// @return nombre de la variable
std::string Variable::getNombre() const{
  return nombre_;
}

/// @brief Devuelve el valor de la variable
/// @return valor de la variable
std::string Variable::getValor() const{
  return valor_;
}

/// @brief Devuelve la línea en la que reside la variable
/// @return la línea en donde se encuentra la variable
int Variable::getLinea() const{
  return linea_;
}
