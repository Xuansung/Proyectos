/*Funcionalidad: Implementación de la Clase BigInteger
 * Fecha: 10/02/2025
 * Autor: Xuan Sun
*/
// Librería
#include "BigInteger.h"

// Constructores

/// @brief Constructor de BigInteger que se puede declara con un int.
/// @param number 
BigInteger::BigInteger(int number) {
  if ( number < 0) {
    negative_ = true;
    unsigned temporal = static_cast<unsigned>(-static_cast<long long>(number));
    number_ = BigUnsigned(temporal);
  } else {
    negative_ = false;
    number_ = BigUnsigned(static_cast<unsigned>(number));
  }
}
/// @brief Constructor por defecto
/// @param bignumber 
BigInteger::BigInteger(const BigUnsigned& bignumber) : number_(bignumber) , negative_(false) {}

/// @brief Constructor copia
/// @param other 
BigInteger::BigInteger(const BigInteger& other) : number_(other.number_), negative_(other.negative_) {}

// Operadores de escritura/lectura
/// @brief Sobrecargar del operador << para poder utilizar la salida estándar
/// @param out 
/// @param bignumber 
/// @return out
std::ostream& operator<<(std::ostream& out, const BigInteger& bignumber) {
  if ( bignumber.negative_) {
    out << "-";
  }
  out << bignumber.number_;
  return out;
}
/// @brief Sobrecarga del operador >> para hacer uso de la entrada estándar.
/// @param in 
/// @param bignumber 
/// @return in
std::istream& operator>>(std::istream& in, BigInteger& bignumber) {
  std::string temporal;
  in >> temporal;  
  bignumber.negative_ = false;
  int start = 0;
  if (temporal[0] == '-') {
    bignumber.negative_ = true;
    start = 1;  // Saltamos el signo para leer solo los dígitos
  }
  for (int i = temporal.size() - 1; i >= start; --i) {
    unsigned char digit = static_cast<unsigned char>(temporal[i] - '0');
    bignumber.number_.setBigUnsigned(digit);
  }
  return in;
}
// Operadores Básicos
/// @brief Sobrecarga del operador = para poder hacer uso de la asignación
/// @param other 
/// @return BigInteger asignado
BigInteger& BigInteger::operator=(const BigInteger& other) {
  number_ = other.number_;
  negative_ = other.negative_;
  return *this;
}
/// @brief Sobrecarga del operador == para comprobar la igualdad
/// @param other 
/// @return 1 si son iguales 0 si no.
bool BigInteger::operator==(const BigInteger& other) const {
  return ((number_ == other.number_) && (negative_ == other.negative_));
}
/// @brief Sobrecarga del operador < para hacer uso de la comparación
/// @param other 
/// @return 1 si es menor que, 0 si no.
bool BigInteger::operator<(const BigInteger& other) const {
  if (negative_ && !other.negative_) {
    return true;
  }
  if (!negative_ && other.negative_) {
    return false;
  }
  if (!negative_ && !other.negative_) {
    return (number_ < other.number_);
  } else {
    return (other.number_ < number_);
  }
}
// Incremento y decremento
/// @brief sobrecargar del ++operador
/// @return ++BigInteger
BigInteger& BigInteger::operator++() { // Pre-incremento
  *this = *this + BigInteger(1);
  return *this;
}
/// @brief Sobrecarga del operador++
/// @param int 
/// @return BigInteger++
BigInteger BigInteger::operator++(int) { // Post-incremento
  BigInteger temporal(*this);
  ++(*this);
  return temporal;
} 
/// @brief Sobrecarga del --operador
/// @return --BigInteger
BigInteger& BigInteger::operator--() { // Pre-decremento
  *this = *this - BigInteger(1);
  return *this;
}
/// @brief Sobrecarga del operador--
/// @param  int
/// @return BigInteger--
BigInteger BigInteger::operator--(int){ // Post-decremento
  BigInteger temporal(*this);
  --(*this);
  return temporal;
}
// Operaciones aritméticas

/// @brief Sobrecarga del operador suma
/// @param other 
/// @return resultado de la suma
BigInteger BigInteger::operator+(const BigInteger& other) const {
  // Caso 1: Mismo signo
  if (negative_ == other.negative_) {
    BigInteger result;
    result.number_ = number_ + other.number_;
    result.negative_ = negative_;
    return result;
  // Caso 2: Signos diferentes
  } else {
    if( other.number_ < number_) {
      BigInteger result;
      result.number_ = number_ - other.number_;
      result.negative_ = negative_;
      return result;
    } else {
      BigInteger result;
      result.number_ = other.number_ - number_;
      result.negative_ = other.negative_;
      return result;
    }
  }
}
/// @brief Sobrecarga del operador -
/// @param other 
/// @return Resultado de la resta.
BigInteger BigInteger::operator-(const BigInteger& other) const {
  BigInteger temporal = other;
  temporal.negative_ = !temporal.negative_;
  return (*this + temporal);
}
/// @brief Sobrecarga del operador +
/// @param other 
/// @return Resultado de la multiplicación
BigInteger BigInteger::operator*(const BigInteger& other) const {
  BigInteger result;
  result.number_ = number_ * other.number_;
  result.negative_ = (negative_ != other.negative_);
  return result;
}
/// @brief Sobrecarga del operador /
/// @param other 
/// @return Resultado de la división
BigInteger BigInteger::operator/(const BigInteger& other) const {
  BigInteger result;
  result.number_ = number_ / other.number_;
  result.negative_ = (negative_ != other.negative_);
  return result;
}
/// @brief Sobrecarga del operador %
/// @param other 
/// @return Resto de la división
BigInteger BigInteger::operator%(const BigInteger& other) const {
  BigInteger result;
  result.number_ = number_ % other.number_;
  result.negative_ = negative_;
  return result;
}
/// @brief Calcula el Máximo común divisor de dos BigInteger
/// @param operand1 
/// @param operand2 
/// @return el máximo común divisor.
BigInteger mcd(const BigInteger& operand1, const BigInteger& operand2) {
  BigInteger abs_operand1 = operand1.abs();
  BigInteger abs_operand2 = operand2.abs();
  
  while (!(abs_operand2 == BigInteger(0))) {
    BigInteger temporal = abs_operand1 % abs_operand2;
    abs_operand1 = abs_operand2;
    abs_operand2 = temporal;
    std::cout << abs_operand1 << " " << abs_operand2  << " " << temporal << std::endl;
  }
  return abs_operand1;
}

// Métodos auxuliares

/// @brief Devuelve el valor absoluto
/// @return Valor Absoluto
BigInteger BigInteger::abs() const {
  BigInteger result(*this);
  result.negative_ = false;
  return result;
}

