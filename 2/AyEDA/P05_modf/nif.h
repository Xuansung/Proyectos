/*
* Autor: Xuan Sun
* Fecha: 22/03/2025
* Resumen: Contiene la clase NIF
*/
// Headers
#ifndef NIF_H
#define NIF_H
// Librerías
#include <ctime>
#include <iostream>

class NIF {
 public:
  // Constructor que crea un DNI aleatorio
  NIF() {
    nif_ = std::rand() % 100000000;
  };
  // Constructor dado por un long
  explicit NIF(long nif) {
    nif_ = nif % 100000000;
  }
  // Sobrecarga del operador de comparación de igualdad.
  bool operator==(const NIF& nif2) const {
    return nif_ == nif2.nif_; 
  }
  bool operator<(const NIF& nif2) const {
    return nif_ < nif2.nif_;
  }
  bool operator>(const NIF& nif2) const {
    return nif_ > nif2.nif_;
  }
  bool operator<=(const NIF& nif2) const {
    return nif_ <= nif2.nif_;
  }
  bool operator>=(const NIF& nif2) const {
    return nif_ >= nif2.nif_;
  }
  // Conversión a long
  operator long() const { return nif_; }
   // Operadores de flujo
  friend std::ostream& operator<<(std::ostream& os, const NIF& nif) {
    os << nif.nif_;
    return os;
  }
  friend std::istream& operator>>(std::istream& is, NIF& nif) {
    long val;
    is >> val;
    nif.nif_ = val % 100000000;
    return is;
  }
 private:
  long nif_;
};

#endif //NIF_H