/*Funcionalidad: Declaración de la Clase BigInteger
 * Fecha: 10/02/2025
 * Autor: Xuan Sun
*/
// Header
#pragma once
// Librerías
#include "BigUnsigned.h"
#include <iostream>
class BigInteger {
 public:
  // Constructores
  BigInteger(int number = 0);
  BigInteger(const BigUnsigned& bignumber);
  BigInteger(const BigInteger& other);
  // Métodos de acceso al signo
  inline bool IsNegative() const { return negative_;}
  inline void setNegative( bool negative) { negative_ = negative;}
  // Operadores de escritura/lectura
  friend std::ostream& operator<<(std::ostream& out, const BigInteger& bignumber);
  friend std::istream& operator>>(std::istream& in, BigInteger& bignumber);
  // Operadores Básicos
  BigInteger& operator=(const BigInteger& other);
  bool operator==(const BigInteger& other) const;
  bool operator<(const BigInteger& other) const;
  // Incremento y decremento
  BigInteger& operator++(); // Pre-incremento
  BigInteger operator++(int); // Post-incremento
  BigInteger& operator--(); // Pre-decremento
  BigInteger operator--(int); // Post-decremento
  // Operaciones aritméticas
  BigInteger operator+(const BigInteger& other) const;
  BigInteger operator-(const BigInteger& other) const;
  BigInteger operator*(const BigInteger& other) const;
  BigInteger operator/(const BigInteger& other) const;
  BigInteger operator%(const BigInteger& other) const;
  friend BigInteger mcd(const BigInteger& operand1, const BigInteger& operand2);
  // Métodos auxiliares
  BigInteger abs() const;
 private:
  BigUnsigned number_;
  bool negative_;
};