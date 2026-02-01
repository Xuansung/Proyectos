/*Funcionalidad: Declaración de la Clase BigUnsigned
 * Fecha: 24/02/2025
 * Autor: Xuan Sun
*/

// Header
#ifndef BIGUNSIGNED_H
#define BIGUNSIGNED_H
// Librerías
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept> // std::runtime_error
// Clase BigUnsigned: Representar números de valores gigantes
template <unsigned char Base>
class BigUnsigned {
 public:
  // Constructores
  /// @brief Constructor por defecto de BigUnsigned
  /// @param number 
  BigUnsigned(unsigned number = 0) {
    // Conversión de un unsigned al vector de dígitos en la base dada.
    if (number == 0) {
      big_unsigned_.push_back(0);
      return;
    }
    while (number > 0) {
      unsigned char digit = number % Base;
      big_unsigned_.push_back(digit);
      number /= Base;
    }
    Removezeros();
  }

  /// @brief Constructor dado por un Unsigned Char
  /// @param value 
  BigUnsigned(const unsigned char* value ) {
    std::vector<unsigned char> aux;
    for (int i = 0; value[i] != '\0'; i++) {
      unsigned char caracter = value[i];
      unsigned char digit = 0;
      // CORRECCIÓN: si '0' <= caracter <= '9'
      if (caracter >= '0' && caracter <= '9') {
        digit = caracter - '0';
      } 
      // CORRECCIÓN: si 'A' <= caracter <= 'Z'
      else if (caracter >= 'A' && caracter <= 'Z') {
        digit = caracter - 'A' + 10;
      } 
      else {
      }
      aux.push_back(digit);
    }
    std::reverse(aux.begin(), aux.end());
    big_unsigned_ = aux;
    Removezeros();
  }
  // Constructor copia
  BigUnsigned(const BigUnsigned<Base>& other) : big_unsigned_(other.big_unsigned_) {}

  // Asignación 
  /// @brief Sobrecarga del operador =
  /// @param other 
  /// @return *this asignado
  BigUnsigned<Base>& operator=(const BigUnsigned<Base>& other) {
    if (this != &other) {
      big_unsigned_ = other.big_unsigned_;
    }
    return *this;
  }

  // Inserción y extracción
  /// @brief Sobrecarga del operador de extracción
  /// @tparam B 
  /// @param out 
  /// @param bignumber 
  /// @return out
  template <unsigned char B>
  friend std::ostream& operator<<(std::ostream& out, const BigUnsigned<B>& bignumber);

  /// @brief Sobrecarga del operador de inserción
  /// @tparam B 
  /// @param in 
  /// @param bignumber 
  /// @return in
  
  template <unsigned char B>
  friend std::istream& operator>>(std::istream& in, BigUnsigned<B>& bignumber);

  // Comparación
  /// @brief Sobrecarga del operador ==
  /// @param other 
  /// @return igualdad de dos BigUnsigned
  bool operator==(const BigUnsigned<Base>& other) const {
    return big_unsigned_ == other.big_unsigned_;
  }

  /// @brief Sobrecarga del operador <
  /// @param bignumber 
  /// @param other 
  /// @return Comparación de dos BigUnsigned
  friend bool operator<(const BigUnsigned<Base>& bignumber, const BigUnsigned<Base>& other) {
    // 1. Compara tamaño
    if (bignumber.big_unsigned_.size() < other.big_unsigned_.size()) return true;
    if (bignumber.big_unsigned_.size() > other.big_unsigned_.size()) return false;
    // 2. Si son del mismo tamaño, comparamos dígito a dígito
    for (int i = static_cast<int>(bignumber.big_unsigned_.size()) - 1; i >= 0; i--) {
    if (bignumber.big_unsigned_[i] < other.big_unsigned_[i]) 
      return true;
    else if (bignumber.big_unsigned_[i] > other.big_unsigned_[i]) 
      return false;
    // si son iguales, se sigue comparando el siguiente dígito menos significativo
    }
    return false;
  }

  // Incremento y decremento
  /// @brief Sobrecarga del ++operador
  /// @return ++*this
  BigUnsigned<Base>& operator++(){ // Pre-incremento
    *this = *this + BigUnsigned<Base>(1);
    return *this;  
  }
  
  /// @brief Sobrecarga del operador++
  /// @param  
  /// @return *this++
  BigUnsigned<Base> operator++(int){ // Post-incremento
    BigUnsigned<Base> temp(*this);
    ++(*this);
    return temp;
  }

  /// @brief Sobrecarga del --operador
  /// @return --*this
  BigUnsigned<Base>& operator--(){ // Pre-decremento
    *this = *this - BigUnsigned<Base>(1);
    return *this;
  }

  /// @brief Sobrecarga del operador--
  /// @param  
  /// @return *this--
  BigUnsigned<Base> operator--(int){ // Post-decremento
  BigUnsigned<Base> temp(*this);
  --(*this);
  return temp;
  }

  // Operadores aritméticos:
  /// @brief Sobrecarga del operador +
  /// @param operand1 
  /// @param operand2 
  /// @return suma de dos BigUnsigned
  friend BigUnsigned<Base> operator+(const BigUnsigned<Base>& operand1, const BigUnsigned<Base>& operand2) {
    BigUnsigned<Base> result;
    result.big_unsigned_.clear();
    const int size = std::max(operand1.big_unsigned_.size(), operand2.big_unsigned_.size());
    unsigned char carry = 0;
    for (int i = 0; i < size || carry; i++) {
      unsigned char sum = carry;
      if (i < operand1.big_unsigned_.size())
        sum += operand1.big_unsigned_[i];
      if (i < operand2.big_unsigned_.size())
        sum += operand2.big_unsigned_[i];
      carry = sum / Base;
      sum = sum % Base;
      result.big_unsigned_.push_back(sum);
    }
    result.Removezeros();
    return result;
  }

  /// @brief Sobrecarga del operador -
  /// @param subtrahend 
  /// @return Resta de dos BigUnsigned
  BigUnsigned<Base> operator-(const BigUnsigned<Base>& subtrahend) const {  
    BigUnsigned<Base> result(*this);
    unsigned char carry = 0;
    for (int i = 0; i < subtrahend.big_unsigned_.size() || carry; ++i) {
      unsigned char minuendo = result.big_unsigned_[i];
      unsigned char sustraendo = (i < subtrahend.big_unsigned_.size()) ? subtrahend.big_unsigned_[i] : 0;
      unsigned short aux = minuendo;
      if (aux >= carry) {
        aux = aux - carry;
      } else {
        aux = aux + Base - carry;
      }
      carry = 0;
      if (aux < sustraendo) {
        aux = aux + Base - sustraendo;
        carry = 1;
      } else {
        aux -= sustraendo;
      }
      result.big_unsigned_[i] = static_cast<unsigned char>(aux);
    }
  result.Removezeros();
  return result;
  }

  /// @brief Sobrecarga del operador *
  /// @param multiply 
  /// @return Producto de dos BigUnsigned
  BigUnsigned<Base> operator*(const BigUnsigned<Base>& multiply) const {
    BigUnsigned<Base> result;
    result.big_unsigned_.assign(big_unsigned_.size() + multiply.big_unsigned_.size(), 0);
    for (int i = 0; i < big_unsigned_.size(); ++i) {
      unsigned int carry = 0;
      for (int j = 0; j < multiply.big_unsigned_.size() || carry; ++j) {
        unsigned long long product = big_unsigned_[i];
        if (j < multiply.big_unsigned_.size()) {
          product *= multiply.big_unsigned_[j];
        } else {
          product = 0;
        }
        unsigned long long temporal = result.big_unsigned_[i + j] + product + carry;
        result.big_unsigned_[i + j] = static_cast<unsigned char>(temporal % Base);
        carry = static_cast<unsigned int>(temporal / Base);
      }
    }
    result.Removezeros();
    return result;
  }

  /// @brief Sobrecarga del operador /
  /// @param dividend 
  /// @param divisor 
  /// @return División de dos BigUnsigned
  friend BigUnsigned<Base> operator/(const BigUnsigned<Base>& dividend, const BigUnsigned<Base>& divisor) {
    // divisor == 0.
    if (divisor == BigUnsigned<Base>(0u)) {
      throw std::runtime_error("Error: división por cero");
    }
    if (dividend < divisor) {
      return BigUnsigned<Base>(0u);
    }
    BigUnsigned<Base> result(0u);
    BigUnsigned<Base> current(0u);
    std::vector<unsigned char> reversed_dividend = dividend.big_unsigned_;
    std::reverse(reversed_dividend.begin(), reversed_dividend.end());
    for (size_t i = 0; i < reversed_dividend.size(); ++i) {
      current.big_unsigned_.insert(current.big_unsigned_.begin(), reversed_dividend[i]);
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

  /// @brief Sobrecarga del operador %
  /// @param divisor 
  /// @return Módulo de dos BigUnsigned
  BigUnsigned<Base> operator%(const BigUnsigned& divisor) const {
    if (*this < divisor)
      return *this;
    // a % b = a - (a / b) * b
    BigUnsigned<Base> division = (*this) / divisor;
    BigUnsigned<Base> mult = division * divisor;
    BigUnsigned<Base> mod = (*this) - mult;
    mod.Removezeros();
    return mod;
  }

  // Métodos auxiliares
  /// @brief Retira los 0 redundantes
  void Removezeros() {
    while (big_unsigned_.size() > 1 && big_unsigned_.back() == 0) {
      big_unsigned_.pop_back();
    }
  }

  /// @brief Modifica el valor de un BigUnsigned
  /// @param digit 
  void setBigUnsigned(const unsigned char& digit) {
    if ( digit >= Base) {
      std::cerr << "Dígito inválido para la base actual" << std::endl;
    }
    big_unsigned_.push_back(digit);
    Removezeros();
  }
 private:
  std::vector<unsigned char> big_unsigned_;
};

// Sobrecarga <<
template <unsigned char B>
std::ostream& operator<<(std::ostream& out, const BigUnsigned<B>& bignumber) {
  // Imprimimos en orden inverso: dígito más significativo primero
  // Convertimos cada dígito a carácter (0..9, A..Z)
  for (int i = bignumber.big_unsigned_.size()-1; i >= 0; --i) {
    unsigned char d = bignumber.big_unsigned_[i];
    if (d < 10) 
      out << char('0' + d);
    else 
      out << char('A' + (d - 10)); // 'A' para dígito 10, etc.
  }
  return out;
}

//Sobrecarga >>
template <unsigned char B>
std::istream& operator>>(std::istream& in, BigUnsigned<B>& bignumber) {
  // Leemos como cadena, parseamos manualmente
  std::string input;
  in >> input;
  // Convertir a vector (inverso)
  // Borramos primero el contenido previo
  bignumber = BigUnsigned<B>(0u); 
  bignumber = BigUnsigned<B>((const unsigned char*) input.c_str());
  return in;
}

#endif // BIGUNSIGNED_H