// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 2: Cadenas y lenguajes
// Autor: Xuan Sun
// Correo: alu010175411@ull.edu.es
// Fecha: 19/09/2024
// Archivo lenguaje.h: cabecera de la clase lenguaje.
// Contiene las declaraciones para la definición de la clase lenguaje.
// Referencias:
// Enlaces de interés
// Historial de revisiones
// 19/09/2024 - Creación (primera versión) del código.
// 22/09/2024 - Desarrollo del código.
// 23/09/2024 - Adjuntar comentarios y corregir errores.

#ifndef LENGUAJE_H
#define LENGUAJE_H
#include "cadena.h"

//Clase Lenguaje
class Lenguaje{
 public:
 //Constructor
  Lenguaje(const std::vector<Cadena>& cadenas);
  //Métodos
  void agregarCadena(const Cadena& cadena);
  std::vector<Cadena> getCadenas() const;
  std::vector<Cadena> Prefijos(const Cadena& cadena) const;
  std::vector<Cadena> Sufijos(const Cadena& cadena) const;
 private:
  //Atributos
  std::vector<Cadena> cadenas_;
};
//Sobrecarga del operador '<<'
std::ostream& operator<<(std::ostream& os, const Lenguaje& lenguaje);

#endif //LENGUAJE_H