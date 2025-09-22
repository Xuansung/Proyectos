// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 2: Cadenas y lenguajes
// Autor: Xuan Sun
// Correo: alu010175411@ull.edu.es
// Fecha: 23/09/2024
// Archivo alfabeto.cc
// Implementacion de las funciones de la clase Alfabeto.
// para poder trabajar con la clase alfabeto con sus métodos y atributos.
// Referencias:
// Enlaces de interés
// Historial de revisiones
// 19/09/2024 - Creación (primera versión) del código.
// 22/09/2024 - Desarrollo del código.
// 23/09/2024 - Adjuntar comentarios y corregir errores.


#include "alfabeto.h"
#include <iostream>

/// @brief Constructor de la clase Alfabeto.
/// @param simbolos 
Alfabeto::Alfabeto(const std::string& simbolos) {
  for(int i = 0; i < simbolos.length(); i++){
    this->simbolos_.insert(simbolos[i]);
  }
}

/// @brief Devuelve la variable simbolos_.
/// @return  simbolos_
std::set<char> Alfabeto::getSimbolo() const{
  return simbolos_;
}

/// @brief Identifica si un símbolo pertenece o no al alfabeto.
/// @param simbolo 
/// @return Pertenece o no al alfabeto.
bool Alfabeto::busca(char simbolo)const{
  return simbolos_.find(simbolo) != simbolos_.end();
}

/// @brief Sobrecarga del operador '<<' para poder imprimir el contenido de un alfabeto.
/// @param os 
/// @param alfabeto 
/// @return os
std::ostream& operator<<(std::ostream& os, const Alfabeto& alfabeto){
  os << "{";
  std::set<char> simbolos = alfabeto.getSimbolo();
  for(auto iterador = simbolos.begin(); iterador != simbolos.end();){
    os << *iterador;
    if(iterador++ != simbolos.end()){
      os << ", ";
    }
  }
  os << "}";
  return os;
}
