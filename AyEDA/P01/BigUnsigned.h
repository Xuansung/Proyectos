/*Funcionalidad: Declaración de la Clase BigUnsigned
 * Fecha: 10/02/2025
 * Autor: Xuan Sun
*/

// Header
#pragma once
// Librerías
#include <iostream>
#include <vector>
// Clase BigUnsigned: Representar números de valores gigantes
class BigUnsigned {
 public:
  // Constructores
  BigUnsigned(unsigned number = 0);
  BigUnsigned(const unsigned char* value );
  BigUnsigned(const BigUnsigned& other);
  // Asignación 
  BigUnsigned& operator=(const BigUnsigned& other);
  // Inserción y extracción
  friend std::ostream& operator<<(std::ostream& out, const BigUnsigned& bignumber);
  friend std::istream& operator>>(std::istream& in, BigUnsigned& bignumber);
  // Comparación
  bool operator==(const BigUnsigned& other) const;
  friend bool operator<(const BigUnsigned& bignumber, const BigUnsigned& other);
  // Incremento y decremento
  BigUnsigned& operator++(); // Pre-incremento
  BigUnsigned operator++(int); // Post-incremento
  BigUnsigned& operator--(); // Pre-decremento
  BigUnsigned operator--(int); // Post-decremento
  // Operadores aritméticos:
  friend BigUnsigned operator+(const BigUnsigned& operand1, const BigUnsigned& operand2);
  BigUnsigned operator-(const BigUnsigned& subtrahend) const;
  BigUnsigned operator*(const BigUnsigned& multiply) const;
  friend BigUnsigned operator/(const BigUnsigned& dividend, const BigUnsigned& divisor);
  BigUnsigned operator%(const BigUnsigned& divisor) const;
  // Métodos auxiliares
  void Removezeros();
  void setBigUnsigned(const unsigned char& digit); 
 private:
  std::vector<unsigned char> big_unsigned_;
};