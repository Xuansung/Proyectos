#include <iostream>

void Square(const int n) {
  for (int i = 0 ; i < n ; i++) {
    for (int j = 0; j < n ; j++) {
      std::cout << n;
    }
    std::cout << std::endl;
  }
}
int main() {
  int number;
  while (std::cin >> number) {
    Square(number);
  }
  return 0;
}