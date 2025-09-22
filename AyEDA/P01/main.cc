/*Funcionalidad: Programa principal para el uso de las clases BigInteger y BigUnsigned
 * Fecha: 10/02/2025
 * Autor: Xuan Sun
*/

#include "BigInteger.h"
int main(){
  BigInteger number1, number2;
  std::cout << "Añada el primer número: ";
  std::cin >> number1;
  std::cout << "\nAñada el segundo número: ";
  std::cin >> number2;
  std::cout << "El primer número es " << number1;
  std::cout << "\nEl segundo número es " << number2 << std::endl;
  if ( number1 < number2)
    std::cout <<"El numero menor es " << number1 << std::endl;
  else {
    std::cout << "El numero menor es " << number2 << std::endl;
  }
  std::cout << "La suma es igual a " << number1 + number2 << std::endl;
  std::cout << "La resta es igual a " << number1 - number2 << std::endl;
  std::cout << "La multiplicación es igual a " << number1 * number2 << std::endl;
  std::cout << "La división es igual a " << number1 / number2 << std::endl;
  std::cout << "El resto de la división es igual a " << number1 % number2 << std::endl;
  std::cout << "Preincremento del primer número " << ++number1 << std::endl;
  std::cout << "Postincremento del primer número " << number1++ << " " << number1 << std::endl;
  std::cout << "Predecremento del primer número " << --number1 << std::endl;
  std::cout << "Postdecremento del primer número " << number1-- << " " << number1 << std::endl;
  std::cout << "El máximo común divisor es " << mcd(number1, number2) << std::endl;
  return 0;
}