// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 5: Construcción de un Autómata Finito
// Autor: Xuan Sun
// Correo: alu010175411@ull.edu.es
// Fecha: 22/10/2024
// Archivo estado.cc
// Contiene las funciones de la clase estado.cc.
// Funciones de la clase Estado.
// Referencias:
// Enlaces de interés
// Historial de revisiones
// 20/10/2024 - Creación (primera versión) del código.
// 22/10/2024 - Última modificación.
#include "estado.h"

//Constructor de la clase
Estado::Estado ( const int& identificacion, const bool& aceptado ) : identificacion_{identificacion}, aceptado_{aceptado} {}

/// @brief Función para obtener la Identificación
/// @return identificacion_
int Estado::getIdentificacion() const {
  return identificacion_;
}

/// @brief Función para obtener si es aceptado o no
/// @return aceptado_
bool Estado::getAceptado() const {
  return aceptado_;
}
/// @brief Comparador de < que para los estados
/// @param otro 
/// @return Comparación <
bool Estado::operator<(const Estado& otro) const {
  return identificacion_ < otro.identificacion_;
}

/// @brief Comparador == para los estados
/// @param otro 
/// @return Comparación ==
bool Estado::operator==(const Estado& otro) const {
  return identificacion_ == otro.identificacion_;
}


