#ifndef BIGINTEGER_H
#define BIGINTEGER_H
#include "BigUnsigned.h"

// Clase BigInteger: Enteros grandes con signo
template <unsigned char Base>
class BigInteger : public BigNumber<Base> {
 public:
  // Constructores
  BigInteger(int number = 0) : negative_(false) {
    if (number < 0) {
      negative_ = true;
      number = -number;
    }
    number_ = BigUnsigned<Base>(static_cast<unsigned>(number));
  }
  BigInteger(const BigUnsigned<Base>& bignumber) : number_(bignumber), negative_(false) {}
  BigInteger(const BigInteger<Base>& other) : number_(other.number_), negative_(other.negative_) {}

  // Métodos de acceso
  inline bool IsNegative() const { return negative_; }
  inline void setNegative(bool negative) { negative_ = negative; }
  inline BigUnsigned<Base> getNumber() const { return number_; }

  /// @brief Valor absoluto de este BigInteger
  /// @return Nuevo BigInteger con el valor absoluto
  BigInteger<Base> abs() const {
    BigInteger<Base> temp(*this);
    temp.negative_ = false;
    return temp;
  }

  // Operadores básicos de asignación y comparación
  BigInteger<Base>& operator=(const BigInteger<Base>& other) {
    if (this != &other) {
      number_ = other.number_;
      negative_ = other.negative_;
    }
    return *this;
  }

  bool operator==(const BigInteger<Base>& other) const {
    return (negative_ == other.negative_) && (number_ == other.number_);
  }

  bool operator<(const BigInteger<Base>& other) const {
    if (negative_ && !other.negative_) 
      return true;
    if (!negative_ && other.negative_) 
      return false;
    if (negative_ && other.negative_) {
      // Ambos negativos: el de mayor magnitud será el menor en valor
      return (other.number_ < number_);
    }
    // Ambos no negativos: comparar magnitudes directamente
    return (number_ < other.number_);
  }

  // Operadores aritméticos (devuelven BigInteger)
  BigInteger<Base> operator+(const BigInteger<Base>& other) const {
    BigInteger<Base> result;
    if (negative_ == other.negative_) {
      // Mismo signo: sumamos magnitudes y conservamos el signo
      result.number_ = number_ + other.number_;
      result.negative_ = negative_;
    } else {
      // Signos diferentes: resta de magnitudes
      if (number_ == other.number_) {
        result.number_ = BigUnsigned<Base>(0u);
        result.negative_ = false;
      } else if (number_ < other.number_) {
        result.number_ = other.number_ - number_;
        result.negative_ = other.negative_;
      } else {
        result.number_ = number_ - other.number_;
        result.negative_ = negative_;
      }
    }
    return result;
  }

  BigInteger<Base> operator-(const BigInteger<Base>& other) const {
    BigInteger<Base> negOther(other);
    negOther.negative_ = !negOther.negative_;  // invertir signo del sustraendo
    return (*this) + negOther;
  }

  BigInteger<Base> operator*(const BigInteger<Base>& other) const {
    BigInteger<Base> result;
    result.number_ = number_ * other.number_;
    // Signo: XOR de los signos
    result.negative_ = (negative_ != other.negative_);
    if (result.number_ == BigUnsigned<Base>(0u)) {
      result.negative_ = false;
    }
    return result;
  }

  BigInteger<Base> operator/(const BigInteger<Base>& other) const {
    if (other.number_ == BigUnsigned<Base>(0u)) {
      throw BigNumberDivisionByZero("División por 0 en BigInteger");
    }
    BigInteger<Base> result;
    result.number_ = number_ / other.number_;
    result.negative_ = (negative_ != other.negative_);
    if (result.number_ == BigUnsigned<Base>(0u)) {
      result.negative_ = false;
    }
    return result;
  }

  BigInteger<Base> operator%(const BigInteger<Base>& other) const {
    if (other.number_ == BigUnsigned<Base>(0u)) {
      throw BigNumberDivisionByZero("División por 0 (módulo) en BigInteger");
    }
    // a % b = a - (a/b)*b
    BigUnsigned<Base> mod_num = number_ % other.number_;
    BigInteger<Base> result(mod_num);
    return result;
  }

  // Función amiga para MCD (máximo común divisor)
  friend BigInteger<Base> mcd(const BigInteger<Base>& operand1, const BigInteger<Base>& operand2) {
    BigInteger<Base> abs_operand1 = operand1.abs();
    BigInteger<Base> abs_operand2 = operand2.abs();
    while (!(abs_operand2.number_ == BigUnsigned<Base>(0u))) {
      BigInteger<Base> temp = abs_operand1 % abs_operand2;
      abs_operand1= abs_operand2;
      abs_operand2 = temp;
    }
    return abs_operand1;
  }

  friend BigInteger<Base> mcd(const BigInteger<Base>& operand1, const BigUnsigned<Base>& operand2) {
    BigInteger<Base> integer_operand2(operand2);
    return mcd(operand1, integer_operand2);
  }

  // Implementaciones polimórficas (BigNumber)
  virtual BigNumber<Base>& add(const BigNumber<Base>& other) const override {
    if (const BigUnsigned<Base>* otherUnsigned = dynamic_cast<const BigUnsigned<Base>*>(&other)) {
      BigInteger<Base> temp(*otherUnsigned);
      BigInteger<Base> result = *this + temp;
      return *(new BigInteger<Base>(result));
    }
    if (const BigInteger<Base>* otherInteger = dynamic_cast<const BigInteger<Base>*>(&other)) {
      BigInteger<Base> result = *this + *otherInteger;
      return *(new BigInteger<Base>(result));
    }
    if (const BigRational<Base>* otherRational = dynamic_cast<const BigRational<Base>*>(&other)) {
      // Interpretar *this como BigRational y delegar
      BigRational<Base> big_rational = static_cast<BigRational<Base>>(*this);
      return big_rational.add(*otherRational);
    }
    throw BigNumberBadDigit("Tipo desconocido en add(BigInteger)");
  }

  virtual BigNumber<Base>& subtract(const BigNumber<Base>& other) const override {
    if (const BigUnsigned<Base>* otherUnsigned = dynamic_cast<const BigUnsigned<Base>*>(&other)) {
      BigInteger<Base> temp(*otherUnsigned);
      BigInteger<Base> result = *this - temp;
      return *(new BigInteger<Base>(result));
    }
    if (const BigInteger<Base>* otherInteger = dynamic_cast<const BigInteger<Base>*>(&other)) {
      BigInteger<Base> result = *this - *otherInteger;
      return *(new BigInteger<Base>(result));
    }
    if (const BigRational<Base>* otherRational = dynamic_cast<const BigRational<Base>*>(&other)) {
      BigRational<Base> big_rational = static_cast<BigRational<Base>>(*this);
      return big_rational.subtract(*otherRational);
    }
    throw BigNumberBadDigit("Tipo desconocido en subtract(BigInteger)");
  }

  virtual BigNumber<Base>& multiply(const BigNumber<Base>& other) const override {
    if (const BigUnsigned<Base>* otherUnsigned = dynamic_cast<const BigUnsigned<Base>*>(&other)) {
      BigInteger<Base> temp(*otherUnsigned);
      BigInteger<Base> result = *this * temp;
      return *(new BigInteger<Base>(result));
    }
    if (const BigInteger<Base>* otherInteger = dynamic_cast<const BigInteger<Base>*>(&other)) {
      BigInteger<Base> result = *this * *otherInteger;
      return *(new BigInteger<Base>(result));
    }
    if (const BigRational<Base>* otherRational = dynamic_cast<const BigRational<Base>*>(&other)) {
      BigRational<Base> big_rational = static_cast<BigRational<Base>>(*this);
      return big_rational.multiply(*otherRational);
    }
    throw BigNumberBadDigit("Tipo desconocido en multiply(BigInteger)");
  }

  virtual BigNumber<Base>& divide(const BigNumber<Base>& other) const override {
    if (const BigUnsigned<Base>* otherUnsigned = dynamic_cast<const BigUnsigned<Base>*>(&other)) {
      BigInteger<Base> temp(*otherUnsigned);
      BigInteger<Base> result = *this / temp;
      return *(new BigInteger<Base>(result));
    }
    if (const BigInteger<Base>* otherInteger = dynamic_cast<const BigInteger<Base>*>(&other)) {
      BigInteger<Base> result = *this / *otherInteger;
      return *(new BigInteger<Base>(result));
    }
    if (const BigRational<Base>* otherRationalional = dynamic_cast<const BigRational<Base>*>(&other)) {
      BigRational<Base> big_rational = static_cast<BigRational<Base>>(*this);
      return big_rational.divide(*otherRationalional);
    }
    throw BigNumberBadDigit("Tipo desconocido en divide(BigInteger)");
  }

  virtual BigNumber<Base>& module(const BigNumber<Base>& other) const override {
    if (const BigUnsigned<Base>* otherUnsigned = dynamic_cast<const BigUnsigned<Base>*>(&other)) {
      BigInteger<Base> temp(*otherUnsigned);
      BigInteger<Base> result = *this % temp;
    }
    if (const BigInteger<Base>* otherInteger = dynamic_cast<const BigInteger<Base>*>(&other)) {
      BigInteger<Base> result = *this % *otherInteger;
      return *(new BigInteger<Base>(result));
    }
    throw BigNumberBadDigit("Tipo desconocido en module(BigInteger)");
  }
  // Operadores de conversión
  virtual operator BigUnsigned<Base>() const override {
    return number_;
  }
  virtual operator BigInteger<Base>() const override {
    return *this;
  }
  virtual operator BigRational<Base>() const override {
    return BigRational<Base>(*this, BigUnsigned<Base>(1));
  }

 protected:
  virtual std::ostream& write(std::ostream& out) const override {
    if (negative_ && !(number_ == BigUnsigned<Base>(0u))) {
      out << "-";
    }
    out << number_;
    return out;
  }
  virtual std::istream& read(std::istream& in) override {
    std::string input;
    in >> input;
    if (input.empty()) {
      *this = BigInteger<Base>(0);
      return in;
    }
    bool is_neg = false;
    size_t idx = 0;
    if (input[0] == '-') { is_neg = true; idx = 1; }
    else if (input[0] == '+') { idx = 1; }
    std::string digits = input.substr(idx);
    try {
      BigUnsigned<Base> tmp((const unsigned char*) digits.c_str());
      number_ = tmp;
      negative_ = (is_neg && !(tmp == BigUnsigned<Base>(0u)));
    } catch (...) {
      throw BigNumberBadDigit("Dígito inválido en BigInteger al leer: " + input);
    }
    return in;
  }

 private:
  BigUnsigned<Base> number_;
  bool negative_;
};

// Sobrecarga de inserción de flujo para BigInteger<B> genérico
template <unsigned char B>
std::ostream& operator<<(std::ostream& out, const BigInteger<B>& bignumber) {
  if (bignumber.IsNegative() && !(bignumber.getNumber() == BigUnsigned<B>(0u))) {
    out << "-";
  }
  out << bignumber.getNumber();
  return out;
}

// Sobrecarga de extracción de flujo para BigInteger<B> genérico
template <unsigned char B>
std::istream& operator>>(std::istream& in, BigInteger<B>& bignumber) {
  std::string input;
  in >> input;
  if (input.empty()) {
    bignumber = BigInteger<B>(0);
    return in;
  }
  bool is_neg = false;
  size_t pos = 0;
  if (input[0] == '-') { is_neg = true; pos = 1; }
  else if (input[0] == '+') { pos = 1; }
  std::string digits = input.substr(pos);
  BigUnsigned<B> tmp;
  tmp = BigUnsigned<B>((const unsigned char*) digits.c_str());
  bignumber = BigInteger<B>(tmp);
  bignumber.setNegative(is_neg && !(tmp == BigUnsigned<B>(0u)));
  return in;
}

#include "BigRational.h"
// Especialización de BigInteger para Base = 2 (binario)
template<>
class BigInteger<2> : public BigNumber<2> {
 public:
  // Constructores
  BigInteger(int number = 0) : negative_(false), bit_(0u) {
    if (number < 0) {
      negative_ = true;
      number = -number;
    }
    bit_ = BigUnsigned<2>(static_cast<unsigned>(number));
  }
  BigInteger(const BigUnsigned<2>& bignumber) : negative_(false), bit_(bignumber) {}
  BigInteger(const BigInteger<2>& other) : negative_(other.negative_), bit_(other.bit_) {}

  // Métodos de acceso
  inline bool IsNegative() const { return negative_; }
  inline void setNegative(bool neg) { negative_ = neg; }
  inline const BigUnsigned<2>& getNumber() const { return bit_; }
  inline void setNumber(const BigUnsigned<2>& number) {bit_ = number;}

  BigInteger<2> abs() const {
    BigInteger<2> temp(*this);
    temp.negative_ = false;
    return temp;
  }

  BigInteger<2>& operator=(const BigInteger<2>& other) {
    if (this != &other) {
      bit_ = other.bit_;
      negative_ = other.negative_;
    }
    return *this;
  }

  bool operator==(const BigInteger<2>& other) const {
    return (negative_ == other.negative_) && (bit_ == other.bit_);
  }
  bool operator<(const BigInteger<2>& other) const {
    if (negative_ && !other.negative_) return true;
    if (!negative_ && other.negative_) return false;
    if (negative_ && other.negative_) {
      return (other.bit_ < bit_);
    }
    return (bit_ < other.bit_);
  }

  // Operadores aritméticos para base 2
  BigInteger<2> operator+(const BigInteger<2>& other) const {
    BigInteger<2> result;
    if (negative_ == other.negative_) {
      result.bit_ = bit_ + other.bit_;
      result.negative_ = negative_;
    } else {
      if (bit_ == other.bit_) {
        result.bit_ = BigUnsigned<2>(0u);
        result.negative_ = false;
      } else if (bit_ < other.bit_) {
        result.bit_ = other.bit_ - bit_;
        result.negative_ = other.negative_;
      } else {
        result.bit_ = bit_ - other.bit_;
        result.negative_ = negative_;
      }
    }
    return result;
  }

  BigInteger<2> operator-(const BigInteger<2>& other) const {
    BigInteger<2> temp(other);
    temp.negative_ = !temp.negative_;
    return (*this) + temp;
  }

  BigInteger<2> operator*(const BigInteger<2>& other) const {
    BigInteger<2> result;
    result.bit_ = bit_ * other.bit_;
    result.negative_ = (negative_ != other.negative_);
    if (result.bit_ == BigUnsigned<2>(0u)) result.negative_ = false;
    return result;
  }

  BigInteger<2> operator/(const BigInteger<2>& other) const {
    if (other.bit_ == BigUnsigned<2>(0u)) {
      throw std::runtime_error("Error: División por cero en BigInteger<2>");
    }
    BigInteger<2> result;
    result.bit_ = bit_ / other.bit_;
    result.negative_ = (negative_ != other.negative_);
    if (result.bit_ == BigUnsigned<2>(0u)) result.negative_ = false;
    return result;
  }

  BigInteger<2> operator%(const BigInteger<2>& other) const {
    if (other.bit_ == BigUnsigned<2>(0u)) {
      throw std::runtime_error("Error: Módulo por cero en BigInteger<2>");
    }
    BigInteger<2> result;
    BigInteger<2> div = (*this) / other;
    BigInteger<2> mult = div * other;
    result = (*this) - mult;
    return result;
  }

  friend BigInteger<2> mcd(const BigInteger<2>& operand1, const BigInteger<2>& operand2) {
    BigInteger<2> abs_operand1 = operand1.abs();
    BigInteger<2> abs_operand2 = operand2.abs();
    while (!(abs_operand2.bit_ == BigUnsigned<2>(0u))) {
      BigInteger<2> tmp = abs_operand1 % abs_operand2;
      abs_operand1 = abs_operand2;
      abs_operand2 = tmp;
    }
    return abs_operand1;
  }

  // Métodos polimórficos
  virtual BigNumber<2>& add(const BigNumber<2>& other) const override {
    if (const BigUnsigned<2>* otherUnsigned = dynamic_cast<const BigUnsigned<2>*>(&other)) {
      BigInteger<2> temp(*otherUnsigned);
      BigInteger<2> result = *this + temp;
      return *(new BigInteger<2>(result));
    }
    if (const BigInteger<2>* otherInteger = dynamic_cast<const BigInteger<2>*>(&other)) {
      BigInteger<2> result = *this + *otherInteger;
      return *(new BigInteger<2>(result));
    }
    if (const BigRational<2>* otherRational = dynamic_cast<const BigRational<2>*>(&other)) {
      // No se soporta directamente BigRational en esta especialización
    }
    throw BigNumberBadDigit("Tipo desconocido en add(BigInteger<2>)");
  }

  virtual BigNumber<2>& subtract(const BigNumber<2>& other) const override {
    if (const BigUnsigned<2>* otherUnsigned = dynamic_cast<const BigUnsigned<2>*>(&other)) {
      BigInteger<2> temp(*otherUnsigned);
      BigInteger<2> result = *this - temp;
      return *(new BigInteger<2>(result));
    }
    if (const BigInteger<2>* otherInteger = dynamic_cast<const BigInteger<2>*>(&other)) {
      BigInteger<2> result = *this - *otherInteger;
      return *(new BigInteger<2>(result));
    }
    if (const BigRational<2>* otherRational = dynamic_cast<const BigRational<2>*>(&other)) {
      // No se soporta directamente BigRational en esta especialización
    }
    throw BigNumberBadDigit("Tipo desconocido en subtract(BigInteger<2>)");
  }

  virtual BigNumber<2>& multiply(const BigNumber<2>& other) const override {
    if (const BigUnsigned<2>* otherUnsigned = dynamic_cast<const BigUnsigned<2>*>(&other)) {
      BigInteger<2> temp(*otherUnsigned);
      BigInteger<2> result = *this * temp;
      return *(new BigInteger<2>(result));
    }
    if (const BigInteger<2>* otherInteger = dynamic_cast<const BigInteger<2>*>(&other)) {
      BigInteger<2> result = *this * *otherInteger;
      return *(new BigInteger<2>(result));
    }
    if (const BigRational<2>* otherRational = dynamic_cast<const BigRational<2>*>(&other)) {
      // No se soporta directamente BigRational en esta especialización
    }
    throw BigNumberBadDigit("Tipo desconocido en multiply(BigInteger<2>)");
  }

  virtual BigNumber<2>& divide(const BigNumber<2>& other) const override {
    if (const BigUnsigned<2>* otherUnsigned = dynamic_cast<const BigUnsigned<2>*>(&other)) {
      BigInteger<2> temp(*otherUnsigned);
      BigInteger<2> result = *this / temp;
      return *(new BigInteger<2>(result));
    }
    if (const BigInteger<2>* otherInteger = dynamic_cast<const BigInteger<2>*>(&other)) {
      BigInteger<2> result = *this / *otherInteger;
      return *(new BigInteger<2>(result));
    }
    if (const BigRational<2>* otherRational = dynamic_cast<const BigRational<2>*>(&other)) {
      // No se soporta directamente BigRational en esta especialización
    }
    throw BigNumberBadDigit("Tipo desconocido en divide(BigInteger<2>)");
  }

  virtual BigNumber<2>& module(const BigNumber<2>& other) const override {
    if (const BigUnsigned<2>* otherUnsigned = dynamic_cast<const BigUnsigned<2>*>(&other)) {
      BigInteger<2> temp(*otherUnsigned);
      BigInteger<2> result = *this % temp;
    }
    if (const BigInteger<2>* otherInteger = dynamic_cast<const BigInteger<2>*>(&other)) {
      BigInteger<2> result = *this % *otherInteger;
      return *(new BigInteger<2>(result));
    }
    if (const BigRational<2>* otherRationalional = dynamic_cast<const BigRational<2>*>(&other)) {
      // No se soporta directamente BigRational en esta especialización
    }
    throw BigNumberBadDigit("Tipo desconocido en module(BigInteger)");
  }

  virtual operator BigUnsigned<2>() const override {
    return bit_;
  }
  virtual operator BigInteger<2>() const override {
    return *this;
  }
  virtual operator BigRational<2>() const override {
    // Convertir a BigRational como this/1
    BigRational<2> result(*this, BigUnsigned<2>(1));
    return result;
  }

 protected:
  virtual std::ostream& write(std::ostream& out) const override {
    if (negative_ && !(bit_ == BigUnsigned<2>(0u))) out << '-';
    out << bit_;
    return out;
  }
  virtual std::istream& read(std::istream& in) override {
    std::string token;
    in >> token;
    if (token.empty()) {
      *this = BigInteger<2>(0);
      return in;
    }
    bool is_neg = false;
    size_t pos = 0;
    if (token[0] == '-') { is_neg = true; pos = 1; }
    else if (token[0] == '+') { pos = 1; }
    std::string digits = token.substr(pos);
    BigUnsigned<2> tmp(0u);
    tmp = BigUnsigned<2>((const unsigned char*) digits.c_str());
    bit_ = tmp;
    negative_ = (is_neg && !(tmp == BigUnsigned<2>(0u)));
    return in;
  }

 private:
  BigUnsigned<2> bit_;
  bool negative_;
};

// Sobrecarga de inserción de flujo para BigInteger<2>
template<>
std::ostream& operator<< <2>(std::ostream& out, const BigInteger<2>& bignumber) {
  if (bignumber.IsNegative() && !(bignumber.getNumber() == BigUnsigned<2>(0u))) {
    out << '-';
  }
  out << bignumber.getNumber();
  return out;
}

// Sobrecarga de extracción de flujo para BigInteger<2>
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
  if (token[0] == '-') { is_neg = true; pos = 1; }
  else if (token[0] == '+') { pos = 1; }
  std::string digits = token.substr(pos);
  BigUnsigned<2> tmp(0u);
  tmp = BigUnsigned<2>((const unsigned char*) digits.c_str());
  bignumber.setNumber(tmp);
  bignumber.setNegative((is_neg && !(tmp == BigUnsigned<2>(0u))));
  return in;
}

#endif // BIGINTEGER_H
