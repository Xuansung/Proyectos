/*Funcionalidad: Implementación de la Clase BigUnsigned
 * Fecha: 10/02/2025
 * Autor: Xuan Sun
*/
// Librerías
#include "BigUnsigned.h"
#include <algorithm>
// Constructores

/// @brief Constructor que tiene como parámetro un número sin signo
/// @param number 
BigUnsigned::BigUnsigned(unsigned number){
  while ( number > 0) {
    big_unsigned_.push_back(static_cast<unsigned char>(number % 10));
    number /= 10;
  }
  
}
/// @brief Constructor que tiene como parámetro una secuencia de unsigned char
/// @param value 
BigUnsigned::BigUnsigned(const unsigned char* value){
  for (const unsigned char* start = value; *start != 0; start++) {
    big_unsigned_.push_back(static_cast<char>(*start));
  }
}
/// @brief Constructor copia
/// @param other 
BigUnsigned::BigUnsigned(const BigUnsigned& other) : big_unsigned_(other.big_unsigned_) {}

// Asignación 
/// @brief Sobrecarga del operador "=" para poder asignar números.
/// @param other 
/// @return asignar el valor del número.
BigUnsigned& BigUnsigned::operator=(const BigUnsigned& other){
  big_unsigned_ = other.big_unsigned_;
  return *this;
}

// Inserción y extracción
/// @brief Sobrecarga del operador "<<" para mostrar en la salida el valor del número.
/// @param out 
/// @param bignumber 
/// @return out
std::ostream& operator<<(std::ostream& out, const BigUnsigned& bignumber){
  if (bignumber.big_unsigned_.size() == 1 && bignumber.big_unsigned_[0] == 0) {
    out << 0;
    return out;
  }
  for (int i = static_cast<int>(bignumber.big_unsigned_.size()) - 1; i >= 0; i--) {
    out << static_cast<char>(bignumber.big_unsigned_[i] + '0');
  }
  return out;
}
/// @brief Sobrecarga del operador ">>" para poder hacer uso de la entrada estándar.
/// @param in 
/// @param bignumber 
/// @return in
std::istream& operator>>(std::istream& in, BigUnsigned& bignumber) {
  std::string temporal;
  in >> temporal;
  for (int i = temporal.size() - 1; i >= 0; i--) {
    unsigned char digit = static_cast<unsigned char>(temporal[i] - '0');
    bignumber.big_unsigned_.push_back(digit);
  }
  return in;
}

// Comparación
/// @brief Sobrecarga del operador "==" para comprobar si dos números son iguales.
/// @param other 
/// @return Si dos valores son iguales o no
bool BigUnsigned::operator==(const BigUnsigned& other) const {
  return big_unsigned_ == other.big_unsigned_;
}
/// @brief Sobrecarga del operador "<" para comprobar si el primer valor es menor que el segundo valor.
/// @param bignumber 
/// @param other 
/// @return Si el primer número es menor que el segundo número
bool operator<(const BigUnsigned& bignumber, const BigUnsigned& other){
  if (bignumber.big_unsigned_.size() < other.big_unsigned_.size()) return true;
  if (bignumber.big_unsigned_.size() > other.big_unsigned_.size()) return false;
  for(int i = static_cast<int>(bignumber.big_unsigned_.size()) - 1; i >= 0; i--) {
    if (bignumber.big_unsigned_[i] < other.big_unsigned_[i]) return true;
    else return false;
  }
  return false;
}
 // Incremento y decremento
BigUnsigned& BigUnsigned::operator++(){ // Pre-incremento
  int carry = 1;
  for (int i = 0; i < big_unsigned_.size(); i++) {
    int temp = big_unsigned_[i] + carry;
    big_unsigned_[i] = temp % 10;
    carry = temp / 10;
    if (!carry) break;
  }
  if (carry) {
    big_unsigned_.push_back(carry);
  }
  return *this;
}
BigUnsigned BigUnsigned::operator++(int){ // Post-incremento
    BigUnsigned temporal(*this);
    ++(*this);
    return temporal;
} 
BigUnsigned& BigUnsigned::operator--(){ // Pre-decremento
  unsigned carry = 1;
  for (unsigned i = 0; i < big_unsigned_.size() && carry; i++) {
    if (big_unsigned_[i] >= carry) {
      big_unsigned_[i] = static_cast<unsigned char>(big_unsigned_[i] - carry);
      carry = 0;
    } else {
      big_unsigned_[i] = static_cast<unsigned char>(10 + big_unsigned_[i] - carry);
      carry = 1;
    }
  }
  return *this;
} 
BigUnsigned BigUnsigned::operator--(int){  // Post-decremento
  BigUnsigned temporal(*this);
  --(*this);
  return temporal;
}
// Operadores aritméticos:
BigUnsigned operator+(const BigUnsigned& operand1, const BigUnsigned& operand2){
  BigUnsigned result;
  const int maxsize =  std::max(operand1.big_unsigned_.size(), operand2.big_unsigned_.size());
  int carry = 0;
  for ( int i = 0; i < maxsize || carry; i++) {
    int sum = carry;
    if (i < operand1.big_unsigned_.size()) sum += operand1.big_unsigned_[i];
    if (i < operand2.big_unsigned_.size()) sum += operand2.big_unsigned_[i];
    result.big_unsigned_.push_back(static_cast<unsigned char>(sum % 10));
    carry = sum / 10;
  }
  return result;
}

BigUnsigned BigUnsigned::operator-(const BigUnsigned& subtrahend) const{
  if (*this < subtrahend )
    std::cerr << "ERROR: El minuendo es menor que el sustraendo" << std::endl;
  BigUnsigned result(*this);
  unsigned borrow = 0;
  for( unsigned i = 0; i < subtrahend.big_unsigned_.size() || borrow; i++) {
    unsigned sub = result.big_unsigned_[i] - borrow - (i < subtrahend.big_unsigned_.size() ? subtrahend.big_unsigned_[i] : 0);
    borrow = 0;
    if ( sub > result.big_unsigned_[i]) {
      sub += 10;
      borrow = 1;
    }
    result.big_unsigned_[i] = static_cast<unsigned char>(sub);
  }
  result.Removezeros();
  return result;
 }


BigUnsigned BigUnsigned::operator*(const BigUnsigned& multiply) const{
  BigUnsigned result;
  // +1 extra para un posible acarreo final
  result.big_unsigned_.assign(big_unsigned_.size() + multiply.big_unsigned_.size() + 1, 0);
  for (int i = 0; i < big_unsigned_.size(); ++i) {
    unsigned long long carry = 0; 
    for (int j = 0; j < multiply.big_unsigned_.size() || carry; ++j) {
      // sum = valor actual + producto + carry
      unsigned long long sum = result.big_unsigned_[i + j] + carry;
      if (j < multiply.big_unsigned_.size()) {
        sum += static_cast<unsigned long long>(big_unsigned_[i]) * multiply.big_unsigned_[j];
      }
      // El dígito resultante y el nuevo carry
      result.big_unsigned_[i + j] = static_cast<unsigned char>(sum % 10);
      carry = sum / 10;
    }
  }
  result.Removezeros();
  return result;
}
BigUnsigned operator/(const BigUnsigned& dividend, const BigUnsigned& divisor){
  BigUnsigned result;
  BigUnsigned currentValue;
  result.big_unsigned_.resize(dividend.big_unsigned_.size() + 1, 0);
  // "Long division" estilo manual, trabajando con los dígitos más significativos primero.
  for (int i = dividend.big_unsigned_.size() - 1; i >= 0; --i) {
    // Insertar el dígito al frente en currentValue
    currentValue.big_unsigned_.insert(currentValue.big_unsigned_.begin(), dividend.big_unsigned_[i]);
    // Eliminar ceros sobrantes en currentValue
    currentValue.Removezeros();
    // Contar cuántas veces divisor cabe en currentValue
    int count = 0;
    while (!(currentValue < divisor)) {
      currentValue = currentValue - divisor;
      count++;
    }
    result.big_unsigned_[i] = static_cast<unsigned char>(count);
  }
  result.Removezeros();
  return result;
}

BigUnsigned BigUnsigned::operator%(const BigUnsigned& divisor) const{
  if (*this < divisor)
    return *this;
  // a % b = a -(a / b) * b
  BigUnsigned division = (*this) / divisor;
  BigUnsigned mult = division * divisor;
  BigUnsigned mod = (*this) - mult;
  return mod;
}

void BigUnsigned::Removezeros() {
 while (big_unsigned_.size() > 1 && big_unsigned_.back() == 0) {
  big_unsigned_.pop_back();
 }
}
void BigUnsigned::setBigUnsigned(const unsigned char& digit) {
  big_unsigned_.push_back(digit);
}