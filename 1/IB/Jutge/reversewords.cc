#include <iostream>
#include <string>
#include <algorithm>

void ReverseWords(const int& size) {
  std::string string;
  int i = 0;
  for (int i = 0; i < size; i++) {
    std::string aux;
    std::cin >> aux;
    string +=  aux + "\n";
  }
  std::reverse(string.begin(), string.end());
  std::cout << string << std::endl;
}

int main() {
  int size;
  std::cin >> size;
  ReverseWords(size);
  return 0;
}