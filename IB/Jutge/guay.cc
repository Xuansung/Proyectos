#include <iostream>

bool EsGuay(int numero) {
  int sumador = 0;
  bool Impar = true;
  while (numero > 0) {
    int componente = numero % 10;
    if (Impar) {
      sumador += componente;
    }
    Impar = !Impar;
    numero /= 10;
  }
return (sumador % 2 == 0);
}

int main() {
  int numero;
  std::cin >> numero;
  if (EsGuay(numero)) {
    std::cout << "ES GUAY" << std::endl;
  } else {
    std::cout << "NO ES GUAY" << std::endl;
  }
  return 0;
}