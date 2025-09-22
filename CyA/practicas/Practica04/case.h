// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 4: Expresiones regulares
// Autor: Xuan Sun
// Correo: alu010175411@ull.edu.es
// Fecha: 09/10/2024
// Archivo case.h
// Contiene las declaraciones de la clase case.
// Cabecera de la clase case.
// Referencias:
// Enlaces de interés
// Historial de revisiones
// 05/10/2024 - Creación (primera versión) del código.
// 07/10/2024 - Revisión del código y añadir detalles y comentarios.

//HEADERS
#ifndef CASE_H
#define CASE_H

//Librerías usadas
#include <string>

//Clase Case
class Case{
 public:
  //Constructor
  Case(const std::string& valor, const int numero_linea);
  //Métodos
  std::string getValor() const;
  int getNumero_linea() const;
 private:
  //Atributos
  const std::string valor_;
  const int numero_linea_;
};

#endif //CASE_H