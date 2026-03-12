/*Funcionalidad: Declaración de la Clase BigInteger
 * Fecha: 24/02/2025
 * Autor: Xuan Sun
*/
// Header
#ifndef BIGINTEGER_H
#define BIGINTEGER_H
// Librerías
#include "BigUnsigned.h"
// Clase BigInteger
template <unsigned char Base>
class BigInteger {
 public:
  // Constructores
  /// @brief Constructor por defecto de BigInteger
  /// @param number 
  BigInteger(int number = 0) : negative_(false) {
    if (number < 0) {
      negative_ = true;
      number = -number;
    }
    number_ = BigUnsigned<Base>(static_cast<unsigned>(number));
  }

  // Construir desde BigUnsigned
  /// @brief Constructor dado por un BigUnsigned
  /// @param bignumber 
  BigInteger(const BigUnsigned<Base>& bignumber) : number_(bignumber), negative_(false) {}

  // Constructor copia
  BigInteger(const BigInteger<Base>& other) : number_(other.number_), negative_(other.negative_) {}
  
  // Métodos auxiliares
  /// @brief Prueba si el BigInteger es negativo o no
  /// @return true si es negativo
  inline bool IsNegative() const { return negative_;}

  /// @brief Modifica el signo del BigInteger
  /// @param negative 
  inline void setNegative( bool negative) { negative_ = negative;}

  /// @brief Poder obtener el atributo privado number_
  /// @return number_ 
  inline BigUnsigned<Base> getNumber() const {return number_;}
  
  // Operadores de escritura/lectura
  /// @brief Sobrecarga del operador de extracción
  /// @tparam B 
  /// @param out 
  /// @param bignumber 
  /// @return out
  template <unsigned char B>
  friend std::ostream& operator<<(std::ostream& out, const BigInteger<B>& bignumber);

  /// @brief Sobrecarga del operador de inserción
  /// @tparam B 
  /// @param in 
  /// @param bignumber 
  /// @return in
  template <unsigned char B>
  friend std::istream& operator>>(std::istream& in, BigInteger<B>& bignumber);

  // Operadores Básicos
  /// @brief Sobrecarga del operador =
  /// @param other 
  /// @return *this asignado
  BigInteger<Base>& operator=(const BigInteger<Base>& other) {
    number_ = other.number_;
    negative_ = other.negative_;
    return *this;
  }

  /// @brief Sobrecarga del operador ==
  /// @param other 
  /// @return igualdad de dos BigInteger
  bool operator==(const BigInteger<Base>& other) const {
    return (negative_ == other.negative_) && (number_ == other.number_);
  }

  /// @brief Sobrecarga del operador <
  /// @param other 
  /// @return Comparación de dos BigUnsigned
  bool operator<(const BigInteger<Base>& other) const {
    // 1) Si uno es negativo y el otro no, el negativo es menor
    if (negative_ && !other.negative_) return true;
    if (!negative_ && other.negative_) return false;
    // 2) Si ambos son negativos, el que tenga mayor valor absoluto es menor
    if (negative_ && other.negative_) {
      return other.number_ < number_; 
    }
    // 3) Ambos positivos => comparar normal
    return number_ < other.number_;
  }

  // Incremento y decremento
  /// @brief Sobrecarga del ++operador
  /// @return ++*this
  BigInteger<Base>& operator++(){ // Pre-incremento
  if (!negative_) {
    // number_++
    ++number_;
  } else {
    BigUnsigned<Base> uno(1);
    if (number_ == uno) {
      number_ = BigUnsigned<Base>(0);
      negative_ = false;
    } else {
      number_ = number_ - uno;
    }
  }
  return *this;
  }  

  /// @brief Sobrecarga del operador++
  /// @param  
  /// @return *this++
  BigInteger<Base> operator++(int) { // Post-incremento
    BigInteger<Base> temporal(*this);
    ++(*this);
    return temporal;
  }

  /// @brief Sobrecarga del --operador
  /// @return --*this
  BigInteger<Base>& operator--(){ // Pre-decremento
    if (!negative_) {
      BigUnsigned<Base> cero(0), uno(1);
      if (number_ == cero) {
        // 0 => -1
        number_ = uno;
        negative_ = true;
      } else {
        number_ = number_ - uno;
      }
    } else {
      BigUnsigned<Base> uno(1);
      number_ = number_ + uno;
    }
    return *this;
  }

  /// @brief Sobrecarga del operador--
  /// @param  
  /// @return *this--
  BigInteger<Base> operator--(int){ // Post-decremento
    BigInteger<Base> temporal(*this);
    --(*this);
    return temporal;
  }

  // Operaciones aritméticas
  /// @brief Sobrecarga del operador +
  /// @param other 
  /// @return suma de dos BigInteger
  BigInteger<Base> operator+(const BigInteger<Base>& other) const {
    BigInteger<Base> result;
    if (negative_ == other.negative_) {
      // (a) mismo signo
      result.number_ = number_ + other.number_;
      result.negative_ = negative_;
    } else {
      // distinto signo
      if (number_ == other.number_) {
        // se anulan
        result.number_ = BigUnsigned<Base>(0u);
        result.negative_ = false;
      } else if (number_ < other.number_) {
        // la magnitud de other es mayor
        result.number_ = other.number_ - number_;
        result.negative_ = other.negative_;
      } else {
        // la magnitud de *this es mayor
        result.number_ = number_ - other.number_;
        result.negative_ = negative_;
      }
    }
    return result;
  }

  /// @brief Sobrecarga del operador -
  /// @param other 
  /// @return  Resta de dos BigInteger
  BigInteger<Base> operator-(const BigInteger<Base>& other) const {
    // a - b = a + (-b)
    BigInteger<Base> temporal(other);
    temporal.negative_ = !temporal.negative_; 
    return (*this) + temporal;
  }

  /// @brief Sobrecarga del operador *
  /// @param other 
  /// @return Producto de dos BigInteger
  BigInteger<Base> operator*(const BigInteger<Base>& other) const {
    BigInteger<Base> result;
    result.number_ = number_ * other.number_;
    result.negative_ = (negative_ != other.negative_);
    // 0 * lo que sea => 0 (signo = positivo)
    if (result.number_ == BigUnsigned<Base>(0u)) {
      result.negative_ = false;
    }
    return result;
  }

  /// @brief Sobrecarga del operador /
  /// @param other 
  /// @return División de dos BigInteger
  BigInteger<Base> operator/(const BigInteger<Base>& other) const {
    if (other.number_ == BigUnsigned<Base>(0u)) {
      throw std::runtime_error("Error: División por cero");
    }
    BigInteger<Base> result;
    result.number_ = number_ / other.number_;
    result.negative_ = (negative_ != other.negative_);
    return result;
  }

  /// @brief Sobrecarga del operador %
  /// @param other 
  /// @return Módulo de dos BigInteger
  BigInteger<Base> operator%(const BigInteger<Base>& other) const {
    if (other.number_ == BigUnsigned<Base>(0u)) {
      throw std::runtime_error("Error: Módulo por cero");
    }
    BigInteger<Base> result;
    result.number_ = number_ % other.number_;
    result.negative_ = negative_;
    return result;
  }

  /// @brief Calcula el máximo común divisor 
  /// @param operand1 
  /// @param operand2 
  /// @return Máximo común divisor de 2 BigInteger
  friend BigInteger<Base> mcd(const BigInteger<Base>& operand1, const BigInteger<Base>& operand2) {
    BigInteger<Base> abs_operand1 = operand1.number_;
    BigInteger<Base> abs_operand2 = operand2.number_;
    BigInteger<Base> temporal;
    while (!(abs_operand2 == BigUnsigned<Base>(0u))) {
      temporal = abs_operand1 % abs_operand2;
      abs_operand1 = abs_operand2;
      abs_operand2 = temporal;
    }
    return abs_operand1;
  }

  // Métodos auxiliares
  /// @brief Calcula el valor absoluto de un BigInteger
  /// @return valor absoluto 
  BigInteger<Base> abs() const {
    BigInteger<Base> temporal(*this);
    temporal.negative_ = false;
    return temporal;
  }

 private:
  BigUnsigned<Base> number_;
  bool negative_;
};

// Sobrecarga <<
template <unsigned char B>
std::ostream& operator<<(std::ostream& out, const BigInteger<B>& bignumber) {
  if ((bignumber.negative_ && !(bignumber.number_ == BigUnsigned<B>(0u)))) {
    out << "-";
  }
  out << bignumber.number_; // aprovechamos operator<< de BigUnsigned
  return out;
}

// Sobrecarga >>
template <unsigned char B>
std::istream& operator>>(std::istream& in, BigInteger<B>& bignumber) {
  // Leemos como cadena. Puede tener signo
  std::string input;
  in >> input;
  if (input.size() == 0) {
    bignumber = BigInteger<B>(0u);
    return in;
  }
  bool is_neg = false;
  size_t pos = 0;
  if (input[0] == '-') {
    is_neg = true;
    pos = 1;
  } else if (input[0] == '+') {
    pos = 1;
  }
  std::string digits = input.substr(pos);
  BigUnsigned<B> tmp(0u);
  tmp = BigUnsigned<B>((const unsigned char*)digits.c_str());
  bignumber.number_ = tmp;
  bignumber.negative_ = is_neg && (!(tmp == BigUnsigned<B>(0u)));
  return in;
}

// Especialización de BigInteger con Base 2
template<>
class BigInteger<2> {
 public:
  // Constructor por defecto 
  BigInteger(int number = 0) : negative_(false), bit_(0u) {
    if (number < 0) {
      negative_ = true;
      number = -number;
    }
    bit_ = BigUnsigned<2>(static_cast<unsigned>(number));
  }
  // Constructor desde un BigUnsigned<2> (lo consideramos no negativo)
  BigInteger(const BigUnsigned<2>& bignumber) : negative_(false), bit_(bignumber) {}
  // Constructor copia
  BigInteger(const BigInteger<2>& other) : negative_(other.negative_), bit_(other.bit_) {}

  // Métodos de acceso
  inline bool IsNegative() const { return negative_; }
  inline void setNegative(bool negative) { negative_ = negative; }
  inline const BigUnsigned<2>& getNumber() const { return bit_;}

  // Asignación
  BigInteger<2>& operator=(const BigInteger<2>& other) {
    if (this != &other) {
      bit_ = other.bit_;
      negative_ = other.negative_;
    }
    return *this;
  }

  //Comparadores
  bool operator==(const BigInteger<2>& other) const {
    return (negative_ == other.negative_) && (bit_ == other.bit_);
  }
  bool operator<(const BigInteger<2>& other) const {
    //Caso 1) negativo < no negativo
    if (negative_ && !other.negative_) return true;
    //Caso 2) no negativo < negativo
    if (!negative_ && other.negative_) return false;
    // Caso 3) ambos negativos
    if (negative_ && other.negative_) {
      //El que tenga mayor bit_ es el menor
      return (other.bit_ < bit_);
    }
    // Caso 4) ambos no negativos
    return (bit_ < other.bit_);
  }

  // Incremento y decremento
  BigInteger<2>& operator++() { // pre-incremento
    if (!negative_) {
      // x >= 0
      ++bit_;  // operador++ de BigUnsigned<2>
    } else {
      // x < 0
      BigUnsigned<2> one(1);
      if (bit_ == one) {
        // -1 + 1 = 0
        bit_ = BigUnsigned<2>(0u);
        negative_ = false;
      } else {
        // -N + 1 => -(N - 1)
        bit_ = bit_ - one; 
      }
    }
    return *this;
  }
  BigInteger<2> operator++(int) { // post-incremento
    BigInteger<2> temp(*this);
    ++(*this);
    return temp;
  }
  BigInteger<2>& operator--() { // pre-decremento
    if (!negative_) {
      // x >= 0
      BigUnsigned<2> zero(0u), one(1);
      if (bit_ == zero) {
        // 0 => -1
        bit_ = one;
        negative_ = true;
      } else {
        bit_ = bit_ - one;
      }
    } else {
      // x < 0 => la magnitud aumenta en 1
      // -N - 1 => -(N+1)
      BigUnsigned<2> one(1);
      bit_ = bit_ + one;
    }
    return *this;
  }
  BigInteger<2> operator--(int) { // post-decremento
    BigInteger<2> temp(*this);
    --(*this);
    return temp;
  }

  // Operaciones aritméticas
  // Suma
  BigInteger<2> operator+(const BigInteger<2>& other) const {
    BigInteger<2> result;
    // Caso (a): mismo signo
    if (negative_ == other.negative_) {
      result.bit_ = bit_ + other.bit_;
      result.negative_ = negative_;
    } else {
      // Distinto signo 
      if (bit_ == other.bit_) {
        // Se anulan => 0
        result.bit_ = BigUnsigned<2>(0u);
        result.negative_ = false;
      } else if (bit_ < other.bit_) {
        // el mayor en magnitud es other
        result.bit_ = other.bit_ - bit_;
        result.negative_ = other.negative_;
      } else {
        // el mayor en magnitud es this
        result.bit_ = bit_ - other.bit_;
        result.negative_ = negative_;
      }
    }
    return result;
  }
  // Resta
  BigInteger<2> operator-(const BigInteger<2>& other) const {
    BigInteger<2> temp(other);
    temp.negative_ = !temp.negative_; // cambiamos el signo de other
    return (*this) + temp;
  }
  // Multiplicación
  BigInteger<2> operator*(const BigInteger<2>& other) const {
    BigInteger<2> result;
    result.bit_ = bit_ * other.bit_;
    // Signo = XOR
    result.negative_ = (negative_ != other.negative_);
    // 0 * lo que sea => 0 (signo se pone false)
    if (result.bit_ == BigUnsigned<2>(0u)) {
      result.negative_ = false;
    }
    return result;
  }
  // División
  BigInteger<2> operator/(const BigInteger<2>& other) const {
    // Comprobación divisor != 0
    if (other.bit_ == BigUnsigned<2>(0u)) {
      throw std::runtime_error("Error: División por cero en BigInteger<2>");
    }
    BigInteger<2> result;
    result.bit_ = bit_ / other.bit_;
    result.negative_ = (negative_ != other.negative_);
    if (result.bit_ == BigUnsigned<2>(0u)) {
      result.negative_ = false;
    }
    return result;
  }
  // Módulo
  BigInteger<2> operator%(const BigInteger<2>& other) const {
    // divisor != 0
    if (other.bit_ == BigUnsigned<2>(0u)) {
      throw std::runtime_error("Error: Módulo por cero en BigInteger<2>");
    }
    BigInteger<2> result;
    // a % b = a - floor(a/b)*b
    BigInteger<2> div = (*this) / other;
    BigInteger<2> mult = div * other;
    result = (*this) - mult;
    return result;
  }
  // MCD
  friend BigInteger<2> mcd(const BigInteger<2>& operand1, const BigInteger<2>& operand2) {
    BigInteger<2> abs_op1 = operand1.abs();
    BigInteger<2> abs_op2 = operand2.abs();
    while (!(abs_op2.bit_ == BigUnsigned<2>(0u))) {
      BigInteger<2> tmp = abs_op1 % abs_op2;
      abs_op1 = abs_op2;
      abs_op2 = tmp;
    }
    // Se devuelve positivo
    abs_op1.negative_ = false;
    return abs_op1;
  }

  // Entrada / Salida
  template<unsigned char B>
  friend std::ostream& operator<<(std::ostream& out, const BigInteger<B>& bignumber);
  template<unsigned char B>
  friend std::istream& operator>>(std::istream& in, BigInteger<B>& bignumber);

  // Método auxiliar
  BigInteger<2> abs() const {
    BigInteger<2> temp(*this);
    temp.negative_ = false;
    return temp;
  }

 private:
  BigUnsigned<2> bit_;  ///< Magnitud en base 2.
  bool negative_;        ///< Indica si el número es negativo.
};

 // Sobrecarga de operator<< para BigInteger<2>
template<>
std::ostream& operator<<<2>(std::ostream& out, const BigInteger<2>& bignumber) {
  if (bignumber.negative_ && !(bignumber.bit_ == BigUnsigned<2>(0u))) {
    out << '-';
  }
  out << bignumber.bit_;
  return out;
}

// Sobrecarga de operator>> para BigInteger<2>
template<>
std::istream& operator>> <2>(std::istream& in, BigInteger<2>& bignumber) {
  std::string token;
  in >> token;
  if (token.empty()) {
    bignumber = BigInteger<2>(0);
    return in;
  }
  bool is_neg = false;
  size_t pos = 0;
  if (token[0] == '-') {
    is_neg = true;
    pos = 1;
  } else if (token[0] == '+') {
    pos = 1;
  }
  std::string digits = token.substr(pos);
  BigUnsigned<2> tmp(0u); 
  tmp = BigUnsigned<2>((const unsigned char*)digits.c_str());
  bignumber.bit_ = tmp;
  bignumber.negative_ = (is_neg && !(tmp == BigUnsigned<2>(0u)));
  return in;
};

#endif // BIGINTEGER_H