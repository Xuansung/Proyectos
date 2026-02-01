// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 5: Construcción de un Autómata Finito
// Autor: Xuan Sun
// Correo: alu010175411@ull.edu.es
// Fecha: 20/10/2024
// Archivo automata_finito.h
// Contiene las declaraciones de la clase AutomataFinito.
// Cabecera de la clase transiciones.
// Referencias:
// Enlaces de interés
// Historial de revisiones
// 20/10/2024 - Creación (primera versión) del código.
// 22/10/2024 - Última modificación.

//Headers
#ifndef AUTOMATA_FINITO_H
#define AUTOMATA_FINITO_H

//Librerías
#include <string>
#include <set>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include "estado.h"
#include <sstream>

//Clase AutomataFinito
class AutomataFinito{
  //Métodos
 public:
  AutomataFinito();
  void setEstadoArranque(const Estado& estado_arranque);
  void AnadirEstados(const Estado& estado);
  void AnadirTransiciones(const Estado& estado_origen, const Estado& estado_destino, char simbolo);
  bool LeerArchivo(const std::string& nombre_archivo);
  bool SimularCadenas(const std::string& cadena);
  // Atributos
 private:
  std::set<char> alfabeto_;
  int num_estados_;
  std::set<Estado> estados_;
  Estado estado_arranque_;
  std::set<Estado> estados_aceptados_;
  std::multimap<std::pair<Estado,char>, Estado> transiciones_;

};


#endif //AUTOMATA_FINITO_H