#include <iostream>
#include <vector>

int main() {
  std::vector<int> numbers;
  int size, number;
  int count = 0;

  // Leer el tamaño del vector
  std::cin >> size;

  // Leer los números
  for (int i = 0; i < size; i++) {
    std::cin >> number;
    numbers.push_back(number);
  }

  // Obtener el último número
  int last_one = numbers[size - 1];

  // Contar las ocurrencias del último número (excluyendo el último elemento)
  for (int i = 0; i < size - 1; i++) {
    if (numbers[i] == last_one) {
      count++;
    }
}

  // Imprimir el conteo
  std::cout << count << std::endl;

  return 0;
}
