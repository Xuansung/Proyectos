
#include "racional.h"

BigRacional::BigRacional(const BigInteger& numerador, const BigUnsigned& denominador) : numerador_(numerador) , denominador_(denominador) {}

std::ostream& operator<<(std::ostream& out, const BigRacional& bignumber) {
  // bignumber.simplify();
  out << bignumber.numerador_ << " /" << bignumber.denominador_ << "i";
  return out;
}

bool BigRacional::operator==(const BigRacional& other) const {
  return ((numerador_ == other.numerador_) && (denominador_ == other.denominador_));
}

BigRacional BigRacional::operator+(const BigRacional& other) const {
  BigRacional result;
  if( denominador_ == other.denominador_) {
    result.numerador_ = numerador_ + other.numerador_;
    result.denominador_ = denominador_;
  } else {
    result.denominador_ = denominador_ * other.denominador_;
    result.numerador_ = numerador_ * other.denominador_ + other.numerador_ * denominador_;
  }
  return result;
}

BigInteger mcd(const BigInteger& operand1, const BigUnsigned& operand2) {
  BigInteger abs_operand1 = operand1.abs();
  BigUnsigned abs_operand2 = operand2;
  BigInteger aux(0);
  while (!(abs_operand2 == aux.number_)) {
    BigInteger temporal = abs_operand1 % abs_operand2;
    abs_operand1 = abs_operand2;
    abs_operand2 = temporal;
    std::cout << abs_operand1 << " " << abs_operand2  << " " << temporal << std::endl;
  }
  return abs_operand1;
}

