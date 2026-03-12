// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 5: Construcción de un Autómata Finito
// Autor: Xuan Sun
// Correo: alu010175411@ull.edu.es
// Fecha: 22/10/2024
// Archivo estado.h
// Contiene las declaraciones de la clase estados.
// Cabecera de la clase estados.
// Referencias:
// Enlaces de interés
// Historial de revisiones
// 20/10/2024 - Creación (primera versión) del código.
// 22/10/2024 - Última modificación.

//HEADERS
#ifndef ESTADO_H
#define ESTADO_H

//Librerías
#include <map>
#include <set>

//Clase Estado
class Estado {
  //Métodos
 public:
  Estado(const int& identificacion, const bool& aceptado);
  int getIdentificacion() const;
  bool getAceptado() const;
  bool operator<(const Estado& otro) const;
  bool operator==(const Estado& otro) const;
  void AnadirTransicion(char simbolo, int estado_destino);
  const std::map<char, std::set<int>>& getTransiciones() const;
  //Atributos
 private:
  int identificacion_;
  bool aceptado_;
  std::map<char, std::set<int>> transiciones_;
};

#endif //ESTADO_H