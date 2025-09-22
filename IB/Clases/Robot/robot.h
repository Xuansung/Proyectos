#pragma once
#include "point2D.h"
#include <string>
#include <iostream>

enum class Direccion {
Norte,
Sur,
Este,
Oeste
};

class Robot {
 public:
  Robot(const std::string& nombre, const Point2D& posicion, const int& cansancio);
  void GiroDerecha();
  void GiroIzquierda();
  void Avanzar();
  void EjecutaSecuencia(const std::string& secuencia);
  std::string getNombre() const {return nombre_;}
  Point2D getPosicion() const {return posicion_;}
  int getCansancio() const { return cansancio_;}
 private:
  std::string nombre_;
  Point2D posicion_;
  Direccion direccion_;
  int cansancio_;
};
std::ostream& operator<<(std::ostream& out, const Robot& robot);