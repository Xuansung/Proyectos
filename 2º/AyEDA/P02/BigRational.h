/*Funcionalidad: Declaración de la Clase BigRational
 * Fecha: 24/02/2025
 * Autor: Xuan Sun
*/
// Header
#ifndef BIGRATIONAL_H
#define BIGRATIONAL_H

// Librerías
#include "BigInteger.h"

// Clase BigRational
template <unsigned char Base>
class BigRational {
  public:
  // Constructor
  BigRational(const BigInteger<Base>& numerator = BigInteger<Base>(0), const BigUnsigned<Base>& denominator = BigUnsigned<Base>(1)) : numerator_(numerator), denominator_(denominator) {Reduce();}
  BigRational(const BigRational& other) : numerator_(other.numerator_), denominator_(other.denominator_) {}

  // Asignación
  /// @brief Sobrecarga del operador =
  /// @param other 
  /// @return *this igualado
  BigRational& operator=(const BigRational& other) {
    if (this != &other) {
      numerator_ = other.numerator_;
      denominator_ = other.denominator_;
    }
    return *this;
  }

  // Comparación: == y <
  /// @brief Sobrecarga del operador ==
  /// @param other 
  /// @return Si 2 BigRational son iguales
  bool operator==(const BigRational& other) const {
    return (numerator_ == other.numerator_) && (denominator_ == other.denominator_);
  }

  /// @brief Sobrecarga del operador <
  /// @param other 
  /// @return Comparación de 2 BigRational
  bool operator<(const BigRational& other) const {
    // a/b < c/d <=> a*d < c*b  (considerando signos)
    BigInteger<Base> cross1 = numerator_ * BigInteger<Base>(other.denominator_);
    BigInteger<Base> cross2 = other.numerator_ * BigInteger<Base>(denominator_);
    return (cross1 < cross2);
  }

  // Operaciones aritméticas
  /// @brief Sobrecarga del operador +
  /// @param other 
  /// @return Suma de dos BigRational
  BigRational operator+(const BigRational& other) const {
    // (a/b) + (c/d) = (ad + bc) / bd
    BigInteger<Base> new_num = numerator_ * BigInteger<Base>(other.denominator_) + other.numerator_ * BigInteger<Base>(denominator_);
    BigUnsigned<Base> new_den = denominator_ * other.denominator_;
    return BigRational(new_num, new_den);
  }

  /// @brief Sobrecarga del operador -
  /// @param other 
  /// @return Resta de dos BigRational
  BigRational operator-(const BigRational& other) const {
    // (a/b) - (c/d) = (ad - bc) / bd
    BigInteger<Base> new_num = numerator_ * BigInteger<Base>(other.denominator_) - other.numerator_ * BigInteger<Base>(denominator_);
    BigUnsigned<Base> new_den = denominator_ * other.denominator_;
    return BigRational(new_num, new_den);
  }

  /// @brief Sobrecarga del operador *
  /// @param other 
  /// @return Producto de dos BigRational
  BigRational operator*(const BigRational& other) const {
    // (a/b) * (c/d) = (ac) / (bd)
    BigInteger<Base> new_num = numerator_ * other.numerator_;
    BigUnsigned<Base> new_den = denominator_ * other.denominator_;
    return BigRational(new_num, new_den);
  }

  /// @brief Sobrecarga del operador /
  /// @param other 
  /// @return Producto de dos BigRational
  BigRational operator/(const BigRational& other) const {
  // (a/b) / (c/d) = (a*d) / (b*c)  (si c=0 => error)
    if (other.numerator_ == BigInteger<Base>(0)) {
      throw std::runtime_error("BigRational::operator/: división por cero (numerador=0)");
    }
    BigInteger<Base> new_num = numerator_ * BigInteger<Base>(other.denominator_);
    BigUnsigned<Base> new_den = denominator_ * other.numerator_.abs().getNumber(); 
    bool final_sign = (numerator_.IsNegative() != other.numerator_.IsNegative());
    BigRational temporal(BigInteger<Base>(new_num), new_den);
    // Ajustamos el signo del numerador (si no es cero)
    if (temporal.numerator_.getNumber() == BigUnsigned<Base>(0u)) {
      temporal.numerator_.setNegative(final_sign);
    } else {
      temporal.numerator_.setNegative(false);
    }
    //temporal.Reduce();
    return temporal;
  }

  // E/O
  /// @brief Sobrecarga del operador de extracción
  /// @param out 
  /// @param rational 
  /// @return out
  friend std::ostream& operator<<(std::ostream& out, const BigRational<Base>& rational) {
    // Formato: num / den
    out << rational.numerator_ << " / " << rational.denominator_;
    return out;
  }
  
  /// @brief Sobrecarga del operador de inserción
  /// @param in 
  /// @param rational 
  /// @return in
  friend std::istream& operator>>(std::istream& in, BigRational<Base>& rational) {
    BigInteger<Base> numerator;
    BigUnsigned<Base> denominator;
    char separate;
    in >> numerator >> separate >> denominator; // lee "num / den"
    if (separate != '/') {
      throw std::runtime_error("Error leyendo BigRational: se esperaba '/'");
    }
    rational = BigRational<Base>(numerator, denominator);
    return in;
  }

 private:
  // Reduce la fracción a su forma irreducible
  void Reduce() {
    // Estandarizamos: el denominador siempre positivo.
    if (denominator_ == BigUnsigned<Base>(0u)) {
      throw std::runtime_error("BigRational::Reduce: denominador == 0");
    }
    if (numerator_ == BigInteger<Base>(0u)) {
      // El número es 0 => la fracción 0/1
      denominator_ = BigUnsigned<Base>(1);
      numerator_.setNegative(false);
      return;
    }
    BigInteger<Base> max_common_divisor = mcd(numerator_, denominator_);
    numerator_ = numerator_ / max_common_divisor;
    denominator_ = denominator_ / max_common_divisor.abs().getNumber();
  }

 private:
  BigInteger<Base> numerator_;
  BigUnsigned<Base> denominator_;
};

#endif // BIGRATIONAL_H