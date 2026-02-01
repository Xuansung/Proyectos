// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 2: Cadenas y lenguajes
// Autor: Xuan Sun
// Correo: alu010175411@ull.edu.es
// Fecha: 23/09/2024
// Archivo cadena.h
// Contiene las declaraciones de la clase Cadena.
// Cabecera de la clase Cadena.
// Referencias:
// Enlaces de interés
// Historial de revisiones
// 19/09/2024 - Creación (primera versión) del código.
// 22/09/2024 - Desarrollo del código.
// 23/09/2024 - Adjuntar comentarios y corregir errores.

#ifndef CADENA_H
#define CADENA_H
#include <iostream>
#include <set>
#include <fstream>
#include <vector>
#include "alfabeto.h"

// Constante '&' que contiene el significado de cadena vacía.
const std::string CADENA_VACIA{'&'};
//Clase Cadena
class Cadena{
 public:
  //Constructor
  Cadena( const std::string& cadena);
  //Métodos
  std::string getCadena() const;
  void setCadena(std::vector<Alfabeto>& cadena);
  int Longitud() const;
  std::string Inversa() const;
  std::string Potencia(const int numero) const;
 private:
   //Atributos
  std::string cadena_;
  const Alfabeto* alfabeto_;
};
//Sobrecarga del operador '<<'
std::ostream& operator<<(std::ostream& os, const Cadena& cadena);

#endif //CADENA_H