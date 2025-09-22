// Header
#pragma once
// Librería
#include "BigInteger.h"
// clase BigRacional
class BigRacional {
 public:
  //Constructor
  BigRacional(const BigInteger& numerador = 0, const BigUnsigned& denominador = 1);
  friend std::ostream& operator<<(std::ostream& out, const BigRacional& bignumber);
  bool operator==(const BigRacional& other) const;
  BigRacional operator+(const BigRacional& other) const;
  // BigRacional simplify() const;
  friend BigInteger mcd(const BigInteger& operand1, const BigUnsigned& operand2);
 private:
  BigInteger numerador_;
  BigUnsigned denominador_;
};