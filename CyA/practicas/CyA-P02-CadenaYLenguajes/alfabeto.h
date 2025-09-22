// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 2: Cadenas y lenguajes
// Autor: Xuan Sun
// Correo: alu010175411@ull.edu.es
// Fecha: 23/09/2024
// Archivo alfabeto.h
// Contiene las declaraciones para la definición de la clase Alfabeto.
// para ... (indicar brevemente el objetivo)
// Referencias:
// Enlaces de interés
// Historial de revisiones
// 19/09/2024 - Creación (primera versión) del código.
// 22/09/2024 - Desarrollo del código.
// 23/09/2024 - Adjuntar comentarios y corregir errores.


#ifndef ALFABETO_H
#define ALFABETO_H

#include <set>
#include <string>
#include <iostream>

//Clase Alfabeto
class Alfabeto{
 public:
  // Constructor
  Alfabeto(const std::string& simbolos);
  //Métodos
  std::set<char> getSimbolo() const;
  bool busca(char simbolo) const;
 private:
 // Atributos
  std::set<char> simbolos_;
};
//Sobrecarga del operador '<<'
std::ostream& operator<<(std::ostream& os, const Alfabeto& alfabeto);

#endif //ALFABETO_H