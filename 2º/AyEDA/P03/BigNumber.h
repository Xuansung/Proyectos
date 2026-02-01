#ifndef BIGNUMBER_H
#define BIGNUMBER_H
#include <exception>
#include <sstream>
#include <string>
#include <iostream>

// Declaraciones adelantadas de clases necesarias
template <unsigned char Base> class BigUnsigned;
template <unsigned char Base> class BigInteger;
template <unsigned char Base> class BigRational;

// Clases de excepciones
class BigNumberException : public std::exception {
 public:
  BigNumberException(const std::string& alert) : alert_(alert) {}
  virtual const char* what() const noexcept override {
    return alert_.c_str();
  }
 private:
  std::string alert_;
};

class BigNumberBadDigit : public BigNumberException {
 public:
  BigNumberBadDigit(const std::string& alert) 
    : BigNumberException("BigNumberBadDigit: " + alert) {}
};

class BigNumberDivisionByZero : public BigNumberException {
 public:
  BigNumberDivisionByZero(const std::string& alert) 
    : BigNumberException("BigNumberDivisionByZero: " + alert) {}
};

// Clase base BigNumber (abstracta, polimórfica)
template <unsigned char Base>
class BigNumber {
 public:
  // Operaciones aritméticas virtuales puras
  virtual BigNumber<Base>& add(const BigNumber<Base>&) const = 0;
  virtual BigNumber<Base>& subtract(const BigNumber<Base>&) const = 0;
  virtual BigNumber<Base>& multiply(const BigNumber<Base>&) const = 0;
  virtual BigNumber<Base>& divide(const BigNumber<Base>&) const = 0;

  // Operadores de conversión virtuales puros
  virtual operator BigUnsigned<Base>() const = 0;
  virtual operator BigInteger<Base>() const = 0;
  virtual operator BigRational<Base>() const = 0;

  // Operadores de inserción y extracción de flujo
  friend std::ostream& operator<<(std::ostream& out, const BigNumber<Base>& number) {
    return number.write(out);
  }
  friend std::istream& operator>>(std::istream& in, BigNumber<Base>& number) {
    return number.read(in);
  }

  // Métodos virtuales de E/S (a implementar en clases derivadas)
  virtual std::ostream& write(std::ostream& out) const = 0;
  virtual std::istream& read(std::istream& in) = 0;

  // Fábrica estática de BigNumber según sufijo de tipo en cadena
  static BigNumber<Base>* create(const char* str) {
    std::string string(str);
    if (string.empty()) {
      return new BigUnsigned<Base>(0u);
    }
    char last = string.back();
    if (last == 'u') {
      // BigUnsigned -> sufijo 'u'
      string.pop_back();
      try {
        return new BigUnsigned<Base>((const unsigned char*) string.c_str());
      } catch (...) {
        throw BigNumberBadDigit("Dígito inválido en BigUnsigned: " + string);
      }
    } else if (last == 'i') {
      // BigInteger -> sufijo 'i'
      string.pop_back();
      try {
        BigInteger<Base> tmp;
        std::stringstream ss;
        ss << string;
        ss >> tmp;  // parseo utilizando operator>> de BigInteger
        return new BigInteger<Base>(tmp);
      } catch (...) {
        throw BigNumberBadDigit("Dígito inválido en BigInteger: " + string);
      }
    } else if (last == 'r') {
      // BigRational -> sufijo 'r' (formato "num/denr")
      string.pop_back();
      size_t pos = string.find('/');
      if (pos == std::string::npos) {
        throw BigNumberBadDigit("Falta '/' en BigRational: " + string);
      }
      std::string numStr = string.substr(0, pos);
      std::string denStr = string.substr(pos + 1);
      try {
        BigInteger<Base> numerator;
        BigUnsigned<Base> denominator;
        {
          std::stringstream ss_num(numStr);
          ss_num >> numerator;
        }
        {
          std::stringstream ss_den(denStr);
          ss_den >> denominator;
        }
        // Construir BigRational a partir de numerador y denominador
        BigRational<Base> bigrational(numerator, denominator);
        return new BigRational<Base>(bigrational);
      } catch (const std::runtime_error& error) {
        std::string msg = error.what();
        if (msg.find("denominador") != std::string::npos || msg.find("División por cero") != std::string::npos) {
          throw BigNumberDivisionByZero("Denominador = 0 en BigRational: " + string);
        }
        throw BigNumberBadDigit("Error parseando BigRational: " + string);
      }
    } else {
      throw BigNumberBadDigit("No se reconoce sufijo en: " + string);
    }
  }

  virtual ~BigNumber() {}
};

#endif // BIGNUMBER_H
