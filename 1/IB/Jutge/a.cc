#include <iostream>
bool find = false;
bool find_a(char caracter) {
  if (caracter == 'a')
    find = true;
  return find;
}
int main() {
  char caracter;
  while (std::cin >> caracter) {
    find_a(caracter);
    if (caracter == '.')
      break;
  }
  if (find_a(caracter)) {
    std::cout << "yes" << std::endl;
  } else {
    std::cout << "no" << std::endl;
  }
  return 0;
}