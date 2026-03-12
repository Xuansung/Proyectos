#include <iostream>
#include <cmath>

void Algorithm(const int base, const int numero) {
  int logaritmo = log10(numero) / log10(base);
  std::cout << logaritmo << std::endl;
}

int main() {
  int base,numero;
  while(std::cin >> base >> numero) {
    if ( base < 2 || numero < 1 )
      break;
    Algorithm(base, numero);
  }
  return 0;
}