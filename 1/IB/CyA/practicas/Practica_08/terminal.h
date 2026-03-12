// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 8: Gramáticas en Forma Normal de Chomsky
// Autor: Xuan Sun
// Correo: alu010175411@ull.edu.es
// Fecha: 05/11/2024
// Archivo terminal.h
// Contiene las declaraciones de la clase Terminal.
// Cabecera de la clase Terminal.
// Referencias:
// Enlaces de interés
// Historial de revisiones
// 01/11/2024 - Creación (primera versión) del código.
// 05/11/2024 - Última modificación.

//Headers
#ifndef TERMINAL_H
#define TERMINAL_H

//Librerías
#include <string>

//Clase Simbolo
class Terminal {
  //Métodos
 public:
  Terminal(const char& valor);
  char getValor() const;
  bool esTerminal() const;
  //Atributos
 private:
  char valor_;
};

#endif //TERMINAL_H