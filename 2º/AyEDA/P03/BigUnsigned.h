/*Funcionalidad: Declaración de la Clase BigUnsigned
 * Fecha: 09/03/2025
 * Autor: Xuan Sun
*/

// Header
#ifndef BIGUNSIGNED_H
#define BIGUNSIGNED_H

// Librerías
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "BigNumber.h"

// Clase BigUnsigned: Representar números de valores gigantes
template <unsigned char Base>
class BigUnsigned : public BigNumber<Base> {
 public:
  // Constructores
  BigUnsigned(unsigned number = 0) {
    if (number == 0) {
      big_unsigned_.push_back(0);
    } else {
      // Descomponer 'number' en dígitos en la base especificada
      while (number > 0) {
        unsigned char digit = number % Base;
        big_unsigned_.push_back(digit);
        number /= Base;
      }
      Removezeros();
    }
  }
  // Constructor dado por un Unsigned Char
  BigUnsigned(const unsigned char* value) {
    std::vector<unsigned char> aux;
    for (int i = 0; value[i] != '\0'; ++i) {
      unsigned char caracter = value[i];
      unsigned char digit;
      // CORRECCIÓN: convertir carácter a dígito según base
      if (caracter >= '0' && caracter <= '9') 
        digit = caracter - '0';
      else if (caracter >= 'A' && caracter <= 'Z') 
        digit = caracter - 'A' + 10;
      else throw BigNumberBadDigit("Carácter inválido");
      if (digit >= Base) {
        throw BigNumberBadDigit(std::string("Dígito '") + (char)caracter + "' fuera de rango para base " + std::to_string(Base));
      }
      aux.push_back(digit);
    }
    std::reverse(aux.begin(), aux.end());  // invertir para almacenar LSB primero
    if (aux.empty()) 
      aux.push_back(0);
    big_unsigned_ = aux;
    Removezeros();
  }
  // Constructor copia
  BigUnsigned(const BigUnsigned<Base>& other) : big_unsigned_(other.big_unsigned_) {}
  
  //Operador de Asignación 
  BigUnsigned<Base>& operator=(const BigUnsigned<Base>& other) {
    if (this != &other) {
      big_unsigned_ = other.big_unsigned_;
    }
    return *this;
  }

  // Operadores de flujo (como funciones amigas plantilla)
  template <unsigned char B>
  friend std::ostream& operator<<(std::ostream& out, const BigUnsigned<B>& bignumber);
  template <unsigned char B>
  friend std::istream& operator>>(std::istream& in, BigUnsigned<B>& bignumber);

  // Operadores de comparación
  bool operator==(const BigUnsigned<Base>& other) const {
    return big_unsigned_ == other.big_unsigned_;
  }

  friend bool operator<(const BigUnsigned<Base>& bignumber, const BigUnsigned<Base>& other) {
    if (bignumber.big_unsigned_.size() < other.big_unsigned_.size()) return true;
    if (bignumber.big_unsigned_.size() > other.big_unsigned_.size()) return false;
    // Si tienen igual número de dígitos, comparar de más significativo a menos
    for (int i = (int)bignumber.big_unsigned_.size() - 1; i >= 0; --i) {
      if (bignumber.big_unsigned_[i] < other.big_unsigned_[i]) return true;
      if (bignumber.big_unsigned_[i] > other.big_unsigned_[i]) return false;
    }
    return false;
  }

  // Operadores de incremento/decremento
  BigUnsigned<Base>& operator++() {  // pre-incremento
    *this = *this + BigUnsigned<Base>(1);
    return *this;
  }
  BigUnsigned<Base> operator++(int) {  // post-incremento
    BigUnsigned<Base> temp(*this);
    ++(*this);
    return temp;
  }
  BigUnsigned<Base>& operator--() {  // pre-decremento
    if (*this == BigUnsigned<Base>(0u)) {
      throw BigNumberBadDigit("Intento de -- en 0 (BigUnsigned)");
    }
    *this = *this - BigUnsigned<Base>(1);
    return *this;
  }
  BigUnsigned<Base> operator--(int) {  // post-decremento
    BigUnsigned<Base> temp(*this);
    --(*this);
    return temp;
  }

  // Operadores aritméticos básicos
  friend BigUnsigned<Base> operator+(const BigUnsigned<Base>& operand1, const BigUnsigned<Base>& operand2) {
    BigUnsigned<Base> result;
    result.big_unsigned_.clear();
    const size_t size = std::max(operand1.big_unsigned_.size(), operand2.big_unsigned_.size());
    unsigned int carry = 0;
    for (size_t i = 0; i < size || carry; ++i) {
      unsigned int sum = carry;
      if (i < operand1.big_unsigned_.size()) sum += operand1.big_unsigned_[i];
      if (i < operand2.big_unsigned_.size()) sum += operand2.big_unsigned_[i];
      carry = sum / Base;
      unsigned char digit = sum % Base;
      result.big_unsigned_.push_back(digit);
    }
    result.Removezeros();
    return result;
  }

  BigUnsigned<Base> operator-(const BigUnsigned<Base>& sub) const {
    if (*this < sub) {
      throw BigNumberBadDigit("Resultado negativo en BigUnsigned");
    }
    BigUnsigned<Base> result(*this);
    unsigned char carry = 0;
    for (size_t i = 0; i < sub.big_unsigned_.size() || carry; ++i) {
      unsigned short minuend = result.big_unsigned_[i];
      unsigned short subtrahend = (i < sub.big_unsigned_.size() ? sub.big_unsigned_[i] : 0);
      int diff = minuend - subtrahend - carry;
      carry = 0;
      if (diff < 0) {
        diff += Base;
        carry = 1;
      }
      result.big_unsigned_[i] = static_cast<unsigned char>(diff);
    }
    result.Removezeros();
    return result;
  }

  BigUnsigned<Base> operator*(const BigUnsigned<Base>& mult) const {
    BigUnsigned<Base> result;
    result.big_unsigned_.assign(big_unsigned_.size() + mult.big_unsigned_.size(), 0);
    for (size_t i = 0; i < big_unsigned_.size(); ++i) {
      unsigned int carry = 0;
      for (size_t j = 0; j < mult.big_unsigned_.size() || carry; ++j) {
        unsigned long long product = result.big_unsigned_[i+j] + (unsigned long long)big_unsigned_[i] * (j < mult.big_unsigned_.size() ? mult.big_unsigned_[j] : 0) + carry;
        result.big_unsigned_[i+j] = static_cast<unsigned char>(product % Base);
        carry = static_cast<unsigned int>(product / Base);
      }
    }
    result.Removezeros();
    return result;
  }

  friend BigUnsigned<Base> operator/(const BigUnsigned<Base>& dividend, const BigUnsigned<Base>& divisor) {
    if (divisor == BigUnsigned<Base>(0u)) {
      throw BigNumberDivisionByZero("División por 0");
    }
    if (dividend < divisor) {
      return BigUnsigned<Base>(0u);
    }
    BigUnsigned<Base> result(0u), current(0u);
    // División dígito a dígito (similar a división larga)
    std::vector<unsigned char> rev = dividend.big_unsigned_;
    std::reverse(rev.begin(), rev.end());
    for (unsigned char digit : rev) {
      current.big_unsigned_.insert(current.big_unsigned_.begin(), digit);
      current.Removezeros();
      unsigned char count = 0;
      while (!(current < divisor)) {
        current = current - divisor;
        ++count;
      }
      result.big_unsigned_.insert(result.big_unsigned_.begin(), count);
    }
    result.Removezeros();
    return result;
  }

  BigUnsigned<Base> operator%(const BigUnsigned<Base>& divisor) const {
    if (*this < divisor) return *this;
    BigUnsigned<Base> quot = *this / divisor;
    BigUnsigned<Base> mult = quot * divisor;
    BigUnsigned<Base> mod = *this - mult;
    mod.Removezeros();
    return mod;
  }

  // Métodos auxiliares
  void Removezeros() {
    while (big_unsigned_.size() > 1 && big_unsigned_.back() == 0) {
      big_unsigned_.pop_back();
    }
  }
  void setBigUnsigned(const unsigned char& digit) {
    if (digit >= Base) {
      throw BigNumberBadDigit("La base no soporta este dígito");
    }
    big_unsigned_.push_back(digit);
    Removezeros();
  }

  // Operaciones polimórficas (implementación de BigNumber)
  virtual BigNumber<Base>& add(const BigNumber<Base>& other) const override {
    if (const BigUnsigned<Base>* otherUnsigned = dynamic_cast<const BigUnsigned<Base>*>(&other)) {
      BigUnsigned<Base> sum = *this + *otherUnsigned;
      return *(new BigUnsigned<Base>(sum));
    }
    if (const BigInteger<Base>* otherInteger = dynamic_cast<const BigInteger<Base>*>(&other)) {
      // Convertir BigUnsigned en BigInteger y delegar
      BigInteger<Base> big_integer = static_cast<BigInteger<Base>>(*this);
      return big_integer.add(*otherInteger);
    }
    if (const BigRational<Base>* otherRational = dynamic_cast<const BigRational<Base>*>(&other)) {
      // Convertir BigUnsigned en BigRational y delegar
      BigRational<Base> big_rational = static_cast<BigRational<Base>>(*this);
      return big_rational.add(*otherRational);
    }
    throw BigNumberBadDigit("Tipo desconocido en add(BigUnsigned)");
  }

  virtual BigNumber<Base>& subtract(const BigNumber<Base>& other) const override {
    if (const BigUnsigned<Base>* otherUnsigned = dynamic_cast<const BigUnsigned<Base>*>(&other)) {
      BigUnsigned<Base> diff = *this - *otherUnsigned;
      return *(new BigUnsigned<Base>(diff));
    }
    if (const BigInteger<Base>* otherInteger = dynamic_cast<const BigInteger<Base>*>(&other)) {
      BigInteger<Base> big_integer = static_cast<BigInteger<Base>>(*this);
      return big_integer.subtract(*otherInteger);
    }
    if (const BigRational<Base>* otherRational = dynamic_cast<const BigRational<Base>*>(&other)) {
      BigRational<Base> big_rational = static_cast<BigRational<Base>>(*this);
      return big_rational.subtract(*otherRational);
    }
    throw BigNumberBadDigit("Tipo desconocido en subtract(BigUnsigned)");
  }

  virtual BigNumber<Base>& multiply(const BigNumber<Base>& other) const override {
    if (const BigUnsigned<Base>* otherUnsigned = dynamic_cast<const BigUnsigned<Base>*>(&other)) {
      BigUnsigned<Base> product = *this * *otherUnsigned;
      return *(new BigUnsigned<Base>(product));
    }
    if (const BigInteger<Base>* otherInteger = dynamic_cast<const BigInteger<Base>*>(&other)) {
      BigInteger<Base> big_integer = static_cast<BigInteger<Base>>(*this);
      return big_integer.multiply(*otherInteger);
    }
    if (const BigRational<Base>* otherRational = dynamic_cast<const BigRational<Base>*>(&other)) {
      BigRational<Base> big_rational = static_cast<BigRational<Base>>(*this);
      return big_rational.multiply(*otherRational);
    }
    throw BigNumberBadDigit("Tipo desconocido en multiply(BigUnsigned)");
  }

  virtual BigNumber<Base>& divide(const BigNumber<Base>& other) const override {
    if (const BigUnsigned<Base>* otherUnsigned = dynamic_cast<const BigUnsigned<Base>*>(&other)) {
      BigUnsigned<Base> result = *this / *otherUnsigned;
      return *(new BigUnsigned<Base>(result));
    }
    if (const BigInteger<Base>* otherInteger = dynamic_cast<const BigInteger<Base>*>(&other)) {
      BigInteger<Base> big_integer = static_cast<BigInteger<Base>>(*this);
      return big_integer.divide(*otherInteger);
    }
    if (const BigRational<Base>* otherRational = dynamic_cast<const BigRational<Base>*>(&other)) {
      BigRational<Base> big_rational = static_cast<BigRational<Base>>(*this);
      return big_rational.divide(*otherRational);
    }
    throw BigNumberBadDigit("Tipo desconocido en divide(BigUnsigned)");
  }

  // Operadores de conversión
  virtual operator BigUnsigned<Base>() const override {
    return *this;
  }
  
  virtual operator BigInteger<Base>() const override {
    return BigInteger<Base>(*this);
  }
  virtual operator BigRational<Base>() const override {
    return BigRational<Base>(BigInteger<Base>(*this), BigUnsigned<Base>(1));
  }

  // Métodos de escritura/lectura (usando operadores de flujo)
  virtual std::ostream& write(std::ostream& out) const override {
    out << *this;
    return out;
  }
  virtual std::istream& read(std::istream& in) override {
    in >> *this;
    return in;
  }

 private:
  std::vector<unsigned char> big_unsigned_;
};

// Sobrecarga de inserción de flujo (<<) para BigUnsigned
template <unsigned char B>
std::ostream& operator<<(std::ostream& out, const BigUnsigned<B>& bignumber) {
  // Imprimir dígitos del más significativo al menos significativo
  for (int i = (int)bignumber.big_unsigned_.size() - 1; i >= 0; --i) {
    unsigned char d = bignumber.big_unsigned_[i];
    if (d < 10) out << char('0' + d);
    else out << char('A' + (d - 10));
  }
  return out;
}

// Sobrecarga de extracción de flujo (>>) para BigUnsigned
template <unsigned char B>
std::istream& operator>>(std::istream& in, BigUnsigned<B>& bignumber) {
  std::string input;
  in >> input;
  if (input.empty()) {
    bignumber = BigUnsigned<B>(0u);
    return in;
  }
  bignumber = BigUnsigned<B>((const unsigned char*) input.c_str());
  return in;
}

#endif // BIGUNSIGNED_H
