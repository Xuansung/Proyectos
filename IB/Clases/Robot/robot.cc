#include "robot.h"

Robot::Robot(const std::string& nombre, const Point2D& posicion, const int& cansancio) : nombre_(nombre), posicion_(posicion), cansancio_(cansancio) {}

void Robot::GiroDerecha() {
  switch (direccion_) {
    case Direccion::Norte :
    direccion_ = Direccion::Este;
    break;
    case Direccion::Este :
    direccion_ = Direccion::Sur;
    break;
    case Direccion::Sur :
    direccion_ = Direccion::Oeste;
    break;
    case Direccion::Oeste :
    direccion_ = Direccion::Norte;
    break;
  }
}

void Robot::GiroIzquierda() {
  switch (direccion_) {
    case Direccion::Norte :
    direccion_ = Direccion::Oeste;
    break;
    case Direccion::Este :
    direccion_ = Direccion::Norte;
    break;
    case Direccion::Sur :
    direccion_ = Direccion::Este;
    break;
    case Direccion::Oeste :
    direccion_ = Direccion::Sur;
    break;
  }
}

void Robot::Avanzar() {
  switch (direccion_)
  {
  case Direccion::Norte:
    posicion_.setY(posicion_.getY() + 1);
    cansancio_++;
    break;
  case Direccion::Este:
    posicion_.setX(posicion_.getX() + 1);
    cansancio_++;
    break;
  case Direccion::Sur:
    posicion_.setY(posicion_.getY() - 1);
    cansancio_++;
    break;
  case Direccion::Oeste:
    posicion_.setX(posicion_.getX() - 1);
    cansancio_++;
    break;
  }
}

void Robot::EjecutaSecuencia(const std::string& secuencia) {
  for (char simbolo : secuencia) {
    switch (simbolo) {
      case 'A':
        Avanzar();
        break;
      case 'D':
        GiroDerecha();
        break;
      case 'I':
        GiroIzquierda();
        break;
    }
  }
}

std::ostream& operator<<(std::ostream& out, const Robot& robot){
  out << robot.getNombre() << " posicion(" << robot.getPosicion().getX() << "," << robot.getPosicion().getY() << ") cansancio " << robot.getCansancio() << std::endl;
  return out;
}
