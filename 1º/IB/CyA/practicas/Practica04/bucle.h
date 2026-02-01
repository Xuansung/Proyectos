// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 4: Expresiones regulares
// Autor: Xuan Sun
// Correo: alu010175411@ull.edu.es
// Fecha: 07/10/2024
// Archivo bucle.h
// Contiene las declaraciones de la clase bucle.
// Cabecera de la clase bucle.
// Referencias:
// Enlaces de interés
// Historial de revisiones
// 05/10/2024 - Creación (primera versión) del código.
// 07/10/2024 - Revisión del código y añadir detalles y comentarios.

//HEADERS
#ifndef BUCLE_H
#define BUCLE_H

#include <string>

//Declaración de la clase bucle.
class Bucle{
 public:
  //Métodos
  Bucle(const std::string& tipo_bucle,const int linea);
  std::string getTipo_bucle() const;
  int getLinea() const;
  //Atributos
 private:
  const std::string tipo_bucle_;
  const int linea_;
};
#endif // BUCLE_H