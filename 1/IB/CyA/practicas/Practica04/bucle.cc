// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 4: Expresiones regulares
// Autor: Xuan Sun
// Correo: alu010175411@ull.edu.es
// Fecha: 07/10/2024
// Archivo bucle.cc
// Contiene los métodos de la clase bucle.cc.
// Funciones de la clase bucle.
// Referencias:
// Enlaces de interés
// Historial de revisiones
// 05/10/2024 - Creación (primera versión) del código.
// 07/10/2024 - Revisión del código y añadir detalles y comentarios.

//Librerías usadas.
#include "bucle.h"


/// @brief Constructor de la clase Bucle.
/// @param tipo_bucle 
/// @param linea 
Bucle::Bucle(const std::string& tipo_bucle, const int linea) : tipo_bucle_(tipo_bucle), linea_(linea) {}

/// @brief Devuelve que tipo de bucle es.
/// @return Tipo del bucle
std::string Bucle::getTipo_bucle() const {
  return tipo_bucle_;
}
/// @brief Devuelve en qué línea está el bucle.
/// @return línea del bucle.
int Bucle::getLinea() const {
  return linea_;
}