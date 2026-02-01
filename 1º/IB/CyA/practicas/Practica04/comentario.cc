// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 4: Expresiones regulares
// Autor: Xuan Sun
// Correo: alu010175411@ull.edu.es
// Fecha: 07/10/2024
// Archivo comentario.cc
// Contiene los métodos de la clase comentario.
// Métodos de la clase comentario.
// Referencias:
// Enlaces de interés
// Historial de revisiones
// 05/10/2024 - Creación (primera versión) del código.
// 07/10/2024 - Revisión del código y añadir detalles y comentarios.

//Librerías usadas
#include "comentario.h"

/// @brief Constructor de la clase Comentario.
/// @param tipo_comentario 
/// @param contenido_comentario 
/// @param linea_inicio 
/// @param linea_final 
Comentario::Comentario(const std::string& tipo_comentario, const std::string& contenido_comentario,const int linea_inicio, const int linea_final) 
: tipo_comentario_(tipo_comentario), contenido_comentario_(contenido_comentario), linea_inicio_(linea_inicio), linea_final_(linea_final) {}

/// @brief Devuelve el contenido del comentario.
/// @return Contenido del comentario
std::string Comentario::getContenido_comentario() const{
  return contenido_comentario_;
}

/// @brief Devuelve la línea en donde empieza el comentario.
/// @return La línea de inicio.
int Comentario::getLineaInicio() const{
  return linea_inicio_;
}
/// @brief Devuelve la línea donde termina el comentario.
/// @return La línea del final.
int Comentario::getLineaFinal() const{
  return linea_final_;
}