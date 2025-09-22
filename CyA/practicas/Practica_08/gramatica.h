// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 8: Gramáticas en Forma Normal de Chomsky
// Autor: Xuan Sun
// Correo: alu010175411@ull.edu.es
// Fecha: 05/11/2024
// Archivo gramatica.h
// Contiene las declaraciones de la clase Gramatica.
// Cabecera de la clase Gramatica.
// Referencias:
// Enlaces de interés
// Historial de revisiones
// 01/11/2024 - Creación (primera versión) del código.
// 05/11/2024 - Última modificación.

//Headers 
#ifndef GRAMATICA_H
#define GRAMATICA_H

//Librerías
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "terminal.h"
#include "noterminal.h"
#include "produccion.h"

class Gramatica {
  //Métodos
 public:
  Gramatica();
  bool CargarArchivo(const std::string& archivo);
  bool GuardarArchivo(const std::string& archivo);
  bool EsTerminal(char simbolo) const;
  bool ProduccionesVacias() const;
  bool ProduccionesUnitarias() const;
  bool EsNoTerminal(const std::string& simbolo) const;
  void ConvertirCNF();
  void ReemplazaTerminalProduccion();
  void DivideProduccion();
  std::string NuevoNoTerminal();
  //Atributos
 private:
  std::vector<char> terminales_;
  std::vector<std::string> noterminales_;
  std::vector<Produccion> producciones_;
  NoTerminal simbolo_arranque_;
};

#endif //GRAMATICA_H