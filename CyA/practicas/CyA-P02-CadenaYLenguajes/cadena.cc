// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 2: Cadenas y lenguajes
// Autor: Xuan Sun
// Correo: alu010175411@ull.edu.es
// Fecha: 23/09/2024
// Archivo cadena.cc: implementación de las funciones.
// Contiene las funciones de la clase cadena.cc.
// Referencias:
// Enlaces de interés
// Historial de revisiones
// 19/09/2024 - Creación (primera versión) del código.
// 22/09/2024 - Desarrollo del código.
// 23/09/2024 - Adjuntar comentarios y corregir errores.

#include "cadena.h"
#include <cassert>

/// @brief Constructor de la clase cadena. También verifica que la cadena sea del alfabeto dado.
/// @param cadena 
/// @param alfabeto 
Cadena::Cadena(const std::string& cadena) : cadena_(cadena) {}


/// @brief Devuelve la cadena.
/// @return cadena_
std::string Cadena::getCadena() const{
  return cadena_;
}

/// @brief Devuelve la longitud de la cadena.
/// @return cadena_.length()
int Cadena::Longitud() const{
  return cadena_.length();
}

/// @brief Devuelve la cadena invertida
/// @return la cadena invertida
std::string Cadena::Inversa() const{
  std::string inversa;
  for(int i = cadena_.size() - 1 ; i >= 0 ;i--){
    inversa.push_back(cadena_[i]);
  }
  return inversa;
}

/// @brief Devuelve la potencia de la cadena dada
/// @param numero 
/// @return potencia
std::string Cadena::Potencia(const int numero) const{
  std::string potencia;
  for(int j = 0; j < numero; j++){
    for(int i = 0; i < cadena_.size(); i++){
      potencia.push_back(cadena_[i]);
    }
  }
  if(!potencia.empty()){
    return potencia;
  } else{
    return CADENA_VACIA;
  }
}

 /// @brief Sobrecarga del operador '<<' para imprimir la cadena.
 /// @param os 
 /// @param cadena 
 /// @return os
 std::ostream& operator<<(std::ostream& os, const Cadena& cadena){
  std::string cadena_vector = cadena.getCadena();
  for(int i = 0; i < cadena.Longitud(); i++){
    os << cadena_vector[i];
  }
  return os;
 }


