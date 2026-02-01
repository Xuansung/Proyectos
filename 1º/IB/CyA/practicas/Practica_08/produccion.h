// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 8: Gramáticas en Forma Normal de Chomsky
// Autor: Xuan Sun
// Correo: alu010175411@ull.edu.es
// Fecha: 05/11/2024
// Archivo produccion.h
// Contiene las declaraciones de la clase Produccion.
// Cabecera de la clase Produccion.
// Referencias:
// Enlaces de interés
// Historial de revisiones
// 01/11/2024 - Creación (primera versión) del código.
// 05/11/2024 - Última modificación.


//Headers
#ifndef PRODUCCION_H
#define PRODUCCION_H
//Librerías
#include <string>
#include <vector>
#include "terminal.h"
#include "noterminal.h"

class Produccion {
  //Métodos
 public:
  Produccion(const NoTerminal& izquierdo);
  const NoTerminal getIzquierdo() const;
  void AnadirSimboloDerecho(const Terminal& simbolo);
  void AnadirSimboloDerecho(const NoTerminal& simbolo);
  int TamanoDerecho() const;
  bool EsBinario() const;
  bool EsTerminalSola() const;
  const std::vector<Terminal>& GetTerminalDerecho() const;
  const std::vector<NoTerminal>& GetNoTerminalDerecho() const;
  //Atributos 
 private:
  NoTerminal izquierdo_;
  std::vector<Terminal> terminal_derecho_;
  std::vector<NoTerminal> no_terminal_derecho_;
};

#endif //PRODUCCION_H