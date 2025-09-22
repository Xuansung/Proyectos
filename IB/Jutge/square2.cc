#include <iostream>

void Square(const int n) {
  int k = -1;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      k += 1;
      if ( k > 9)
        k = 0;
      std::cout << k;
    }
    std::cout << std::endl;
  }
}

int main() {
  int number;
  bool first = true; // Para controlar la primera salida
  while (std::cin >> number) {
    if (!first) {
      std::cout << std::endl; // Línea en blanco entre bloques
    }
    first = false;
    Square(number);
  }
  return 0;
}
