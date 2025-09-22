/*
* Autor: Xuan Sun
* Fecha: 22/03/2025
* Resumen: Contiene la clase matricula
*/
// Headers
#ifndef MATRICULA_H
#define MATRICULA_H
// Librerías
#include <ctime>
#include <iostream>
#include <string>

class Matricula {
 public:
  // Constructor 
  Matricula(std::string identificador) : identificador_(identificador) {}
  // Sobrecarga del operador de comparación de igualdad.
  bool operator==(const Matricula& coche2) const {
    return identificador_ == coche2.identificador_; 
  }
  // Conversión a long
  operator long() const {
    long val = 0;
    for(int i = 0; i < identificador_.size(); i++) {
      unsigned char caracter = identificador_[i];
      unsigned char digit;
      if (caracter >= '0' && caracter <= '9') 
        digit = caracter - '0';
      else if (caracter >= 'A' && caracter <= 'Z') 
        digit = caracter - 'A' + 10;
      val += digit;
    }
    return val;
   }
  void setMarca(const std::string& marca) {
    marca_ = marca;
  }
   // Operadores de flujo
  friend std::ostream& operator<<(std::ostream& os, const Matricula& coche) {
    os << coche.identificador_ << " " <<coche.marca_;
    return os;
  }
  friend std::istream& operator>>(std::istream& is, Matricula& coche) {
    std::string matricula;
    std::string marca;
    is >> matricula;
    coche.identificador_ = matricula;
    return is;
  }
 private:
  std::string identificador_;
  std::string marca_;
};

#endif //MATRICULA.H