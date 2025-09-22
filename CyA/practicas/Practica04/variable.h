// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 4: Expresiones regulares
// Autor: Xuan Sun
// Correo: alu010175411@ull.edu.es
// Fecha: 07/10/2024
// Archivo variable.h
// Contiene las declaraciones de la clase variable.
// Cabecera de la clase variable.
// Referencias:
// Enlaces de interés
// Historial de revisiones
// 05/10/2024 - Creación (primera versión) del código.
// 07/10/2024 - Revisión del código y añadir detalles y comentarios.

//HEADERS
#ifndef VARIABLE_H
#define VARIABLE_H
//Librerías usadas
#include <string>

//Clase Variable
class Variable{
 public:
  //Métodos
  Variable(const std::string& tipo_variable,const std::string& nombre, const std::string& valor, const int linea);
  std::string getTipo_variable() const;
  std::string getNombre() const;
  std::string getValor() const;
  int getLinea() const;
  //Atributos
  private:
  std::string tipo_variable_;
  std::string nombre_;
  std::string valor_;
  int linea_;
};

#endif // VARIABLE_H