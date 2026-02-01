#include <iostream>

void print(int s, char c, int n) {
  for (int i = 0; i < s; i++) {
    std::cout << " ";
  }
  for (int i = 0; i < n; i++) {
    std::cout << c;
  }
  std::cout << std::endl;
}