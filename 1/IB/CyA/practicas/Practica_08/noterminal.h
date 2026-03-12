// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 8: Gramáticas en Forma Normal de Chomsky
// Autor: Xuan Sun
// Correo: alu010175411@ull.edu.es
// Fecha: 05/11/2024
// Archivo noterminal.h
// Contiene las declaraciones de la clase NoTerminal.
// Cabecera de la clase NoTerminal.
// Referencias:
// Enlaces de interés
// Historial de revisiones
// 01/11/2024 - Creación (primera versión) del código.
// 05/11/2024 - Última modificación.

//Headers
#ifndef NO_TERMINAL_H
#define NO_TERMINAL_H
//Librerías
#include <string>

class NoTerminal {
  //Métodos
 public:
  NoTerminal()= default;
  NoTerminal(const std::string& valor);
  const std::string& getValor() const;
  bool EsTerminal() const;
  //Atributos
 private:
  std::string valor_;
};

#endif //NO_TERMINAL_H