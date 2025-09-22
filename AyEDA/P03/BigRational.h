#ifndef BIGRATIONAL_H
#define BIGRATIONAL_H
#include "BigInteger.h"

// Clase BigRational: números racionales grandes (fracción de BigInteger/Base y BigUnsigned/Base)
template <unsigned char Base>
class BigRational : public BigNumber<Base> {
 public:
  // Constructores
  BigRational(const BigInteger<Base>& numerator = BigInteger<Base>(0), const BigUnsigned<Base>& denominator = BigUnsigned<Base>(1)) : numerator_(numerator), denominator_(denominator) {
    Reduce();
  }
  BigRational(const BigRational<Base>& other) : numerator_(other.numerator_), denominator_(other.denominator_) {}

  BigRational<Base>& operator=(const BigRational<Base>& other) {
    if (this != &other) {
      numerator_ = other.numerator_;
      denominator_ = other.denominator_;
    }
    return *this;
  }

  // Operadores de comparación
  bool operator==(const BigRational<Base>& other) const {
    return (numerator_ == other.numerator_) && (denominator_ == other.denominator_);
  }

  bool operator<(const BigRational<Base>& other) const {
    // a/b < c/d  <=>  a*d < c*b 
    BigInteger<Base> cross1 = numerator_ * BigInteger<Base>(other.denominator_);
    BigInteger<Base> cross2 = other.numerator_ * BigInteger<Base>(denominator_);
    return (cross1 < cross2);
  }

  // Operadores aritméticos
  BigRational<Base> operator+(const BigRational<Base>& other) const {
    // (a/b) + (c/d) = (ad + bc) / bd
    BigInteger<Base> cross1 = numerator_ * BigInteger<Base>(other.denominator_);
    BigInteger<Base> cross2 = other.numerator_ * BigInteger<Base>(denominator_);
    BigInteger<Base> new_num = cross1 + cross2;
    BigUnsigned<Base> new_den = denominator_ * other.denominator_;
    return BigRational<Base>(new_num, new_den);
  }

  BigRational<Base> operator-(const BigRational<Base>& other) const {
    // (a/b) - (c/d) = (a*d - c*b) / bd
    BigInteger<Base> cross1 = numerator_ * BigInteger<Base>(other.denominator_);
    BigInteger<Base> cross2 = other.numerator_ * BigInteger<Base>(denominator_);
    BigInteger<Base> new_num = cross1 - cross2;
    BigUnsigned<Base> new_den = denominator_ * other.denominator_;
    return BigRational<Base>(new_num, new_den);
  }

  BigRational<Base> operator*(const BigRational<Base>& other) const {
    // (a/b) * (c/d) = (a*c) / (b*d)
    BigInteger<Base> new_num = numerator_ * other.numerator_;
    BigUnsigned<Base> new_den = denominator_ * other.denominator_;
    return BigRational<Base>(new_num, new_den);
  }

  BigRational<Base> operator/(const BigRational<Base>& other) const {
    // (a/b) / (c/d) = (a*d) / (b*c)
    if (other.numerator_ == BigInteger<Base>(0u)) {
      throw std::runtime_error("Error: División por cero en BigRational");
    }
    BigInteger<Base> new_num = numerator_ * BigInteger<Base>(other.denominator_);
    BigUnsigned<Base> absDen = other.numerator_.getNumber();
    BigUnsigned<Base> new_den = denominator_ * absDen;
    if (other.numerator_.IsNegative()) {
      new_num.setNegative(!new_num.IsNegative());
    }
    return BigRational<Base>(new_num, new_den);
  }

  // Operaciones polimórficas (BigNumber)
  virtual BigNumber<Base>& add(const BigNumber<Base>& other) const override {
    if (const BigRational<Base>* otherRational = dynamic_cast<const BigRational<Base>*>(&other)) {
      BigInteger<Base> cross1 = numerator_ * BigInteger<Base>(otherRational->denominator_);
      BigInteger<Base> cross2 = otherRational->numerator_ * BigInteger<Base>(denominator_);
      BigInteger<Base> newNumerator = cross1 + cross2;
      BigUnsigned<Base> newDenominator = denominator_ * otherRational->denominator_;
      return *(new BigRational<Base>(newNumerator, newDenominator));
    }
    if (const BigInteger<Base>* otherInteger = dynamic_cast<const BigInteger<Base>*>(&other)) {
      // (a/b) + (c/1) = (a + c*b) / b
      BigInteger<Base> newNumerator = numerator_ + (*otherInteger) * BigInteger<Base>(denominator_);
      BigUnsigned<Base> newDenominator = denominator_;
      return *(new BigRational<Base>(newNumerator, newDenominator));
    }
    if (const BigUnsigned<Base>* otherUnsigned = dynamic_cast<const BigUnsigned<Base>*>(&other)) {
      // (a/b) + (c/1) con c >= 0
      BigInteger<Base> otherInteger(*otherUnsigned);
      BigInteger<Base> newNumerator = numerator_ + otherInteger * BigInteger<Base>(denominator_);
      BigUnsigned<Base> newDenominator = denominator_;
      return *(new BigRational<Base>(newNumerator, newDenominator));
    }
    throw BigNumberBadDigit("Tipo desconocido en add(BigRational)");
  }

  virtual BigNumber<Base>& subtract(const BigNumber<Base>& other) const override {
    if (const BigRational<Base>* otherRational = dynamic_cast<const BigRational<Base>*>(&other)) {
      BigInteger<Base> cross1 = numerator_ * BigInteger<Base>(otherRational->denominator_);
      BigInteger<Base> cross2 = otherRational->numerator_ * BigInteger<Base>(denominator_);
      BigInteger<Base> newNumerator = cross1 - cross2;
      BigUnsigned<Base> newDenominator = denominator_ * otherRational->denominator_;
      return *(new BigRational<Base>(newNumerator, newDenominator));
    }
    if (const BigInteger<Base>* otherInteger = dynamic_cast<const BigInteger<Base>*>(&other)) {
      BigInteger<Base> newNumerator = numerator_ - (*otherInteger) * BigInteger<Base>(denominator_);
      BigUnsigned<Base> newDenominator = denominator_;
      return *(new BigRational<Base>(newNumerator, newDenominator));
    }
    if (const BigUnsigned<Base>* otherUnsigned = dynamic_cast<const BigUnsigned<Base>*>(&other)) {
      BigInteger<Base> otherInteger(*otherUnsigned);
      BigInteger<Base> newNumerator = numerator_ - otherInteger * BigInteger<Base>(denominator_);
      BigUnsigned<Base> newDenominator = denominator_;
      return *(new BigRational<Base>(newNumerator, newDenominator));
    }
    throw BigNumberBadDigit("Tipo desconocido en subtract(BigRational)");
  }

  virtual BigNumber<Base>& multiply(const BigNumber<Base>& other) const override {
    if (const BigRational<Base>* otherRational = dynamic_cast<const BigRational<Base>*>(&other)) {
      BigInteger<Base> newNumerator = numerator_ * otherRational->numerator_;
      BigUnsigned<Base> newDenominator = denominator_ * otherRational->denominator_;
      return *(new BigRational<Base>(newNumerator, newDenominator));
    }
    if (const BigInteger<Base>* otherInteger = dynamic_cast<const BigInteger<Base>*>(&other)) {
      BigInteger<Base> newNumerator = numerator_ * (*otherInteger);
      BigUnsigned<Base> newDenominator = denominator_;
      return *(new BigRational<Base>(newNumerator, newDenominator));
    }
    if (const BigUnsigned<Base>* otherUnsigned = dynamic_cast<const BigUnsigned<Base>*>(&other)) {
      BigInteger<Base> otherInteger(*otherUnsigned);
      BigInteger<Base> newNumerator = numerator_ * otherInteger;
      BigUnsigned<Base> newDenominator = denominator_;
      return *(new BigRational<Base>(newNumerator, newDenominator));
    }
    throw BigNumberBadDigit("Tipo desconocido en multiply(BigRational)");
  }

  virtual BigNumber<Base>& divide(const BigNumber<Base>& other) const override {
    if (const BigRational<Base>* otherRational = dynamic_cast<const BigRational<Base>*>(&other)) {
      if (otherRational->numerator_ == BigInteger<Base>(0u)) {
        throw BigNumberDivisionByZero("División por 0 en BigRational");
      }
      BigInteger<Base> newNumerator = numerator_ * BigInteger<Base>(otherRational->denominator_);
      BigUnsigned<Base> absDen = otherRational->numerator_.getNumber();
      BigUnsigned<Base> newDenominator = denominator_ * absDen;
      BigInteger<Base> newNumAdj(newNumerator);
      if (otherRational->numerator_.IsNegative()) {
        newNumAdj.setNegative(!newNumAdj.IsNegative());
      }
      return *(new BigRational<Base>(newNumAdj, newDenominator));
    }
    if (const BigInteger<Base>* otherInteger = dynamic_cast<const BigInteger<Base>*>(&other)) {
      if (*otherInteger == BigInteger<Base>(0u)) {
        throw BigNumberDivisionByZero("División por 0 en BigRational");
      }
      BigUnsigned<Base> absDen = otherInteger->getNumber();
      BigUnsigned<Base> newDenominator = denominator_ * absDen;
      BigInteger<Base> newNumerator(numerator_);
      if (otherInteger->IsNegative()) {
        newNumerator.setNegative(!newNumerator.IsNegative());
      }
      return *(new BigRational<Base>(newNumerator, newDenominator));
    }
    if (const BigUnsigned<Base>* otherUnsigned = dynamic_cast<const BigUnsigned<Base>*>(&other)) {
      if (*otherUnsigned == BigUnsigned<Base>(0u)) {
        throw BigNumberDivisionByZero("División por 0 en BigRational");
      }
      BigInteger<Base> newNumerator(numerator_);
      BigUnsigned<Base> newDenominator = denominator_ * (*otherUnsigned);
      return *(new BigRational<Base>(newNumerator, newDenominator));
    }
    throw BigNumberBadDigit("Tipo desconocido en divide(BigRational)");
  }

  // Operadores de conversión
  virtual operator BigUnsigned<Base>() const override {
    return numerator_.getNumber();  // valor absoluto del numerador
  }
  virtual operator BigInteger<Base>() const override {
    // Parte entera de la fracción
    BigInteger<Base> result = numerator_ / BigInteger<Base>(denominator_);
    return result;
  }
  virtual operator BigRational<Base>() const override {
    return *this;
  }

 protected:
  virtual std::ostream& write(std::ostream& out) const override {
    out << numerator_ << "/" << denominator_;
    return out;
  }
  virtual std::istream& read(std::istream& in) override {
    // Leemos en formato "numerador/denominador"
    std::string input;
    in >> input;
    size_t slashPos = input.find('/');
    if (slashPos == std::string::npos) {
      throw BigNumberBadDigit("No se encontró '/' leyendo BigRational");
    }
    std::string numStr = input.substr(0, slashPos);
    std::string denStr = input.substr(slashPos + 1);
    std::stringstream ssNum(numStr);
    std::stringstream ssDen(denStr);
    BigInteger<Base> num;
    BigUnsigned<Base> den;
    ssNum >> num;
    ssDen >> den;
    numerator_ = num;
    denominator_ = den;
    Reduce();
    return in;
  }

 private:
  // Simplificar la fracción a su forma irreducible
  void Reduce() {
    // Asegurar que el denominador no sea 0
    if (denominator_ == BigUnsigned<Base>(0u)) {
      throw BigNumberDivisionByZero("BigRational::Reduce: denominador == 0");
    }
    if (numerator_ == BigInteger<Base>(0u)) {
      // 0/x = 0/1
      denominator_ = BigUnsigned<Base>(1);
      numerator_.setNegative(false);
      return;
    }
    BigInteger<Base> gcd = mcd(numerator_, denominator_);
    numerator_ = numerator_ / gcd;
    denominator_ = denominator_ / gcd.getNumber();
    // El denominador queda positivo (BigUnsigned siempre >= 0).
    // Si el numerador era negativo, el signo queda en numerator_.
  }

  BigInteger<Base> numerator_;
  BigUnsigned<Base> denominator_;
};

#endif // BIGRATIONAL_H
