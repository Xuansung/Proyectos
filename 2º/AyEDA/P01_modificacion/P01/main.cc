/*Funcionalidad: Programa principal para el uso de las clases BigInteger y BigUnsigned
 * Fecha: 10/02/2025
 * Autor: Xuan Sun
*/

//Librería
#include "racional.h"

int main() {
  BigRacional number1(122543, 65443) , number2(44524, 34542);
  std::cout << "El primer racional es " << number1 << std::endl;
  std::cout << "El segundo racional es " << number2 << std::endl;
  if (number1 == number2) {
    std::cout << "Los números son iguales" << std::endl;
  } else {
    std::cout << "LOs números son diferentes" << std::endl;
  }
  std::cout << "La suma de los números es " << number1 + number2 << std::endl;
  
  return 0;
}