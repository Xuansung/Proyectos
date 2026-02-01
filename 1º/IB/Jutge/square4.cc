#include <iostream>

void Square(const int n) {
  for (int l = 0; l < n; l++) {
    int k = -1;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        k += 1;
        if (k > 9)
          k = 0;
        std::cout << k;
      }
      std::cout << std::endl;
    }
    // Print a blank line only if it's not the last square
    if (l < n - 1) {
      std::cout << std::endl;
    }
  }
}

int main() {
  int number;
  std::cin >> number;
  Square(number);
  return 0;
}
