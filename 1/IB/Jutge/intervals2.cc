#include <iostream>

void Intervals(const int a1, const int a2, const int b1, const int b2) {
  if ( a1 == b1 && a2 == b2) {
    std::cout << "=" << std::endl;
  } else if (a1 >= b1 && a2 <= b2) {
    std::cout << "1" << std::endl;
  } else if ( b1 >= a1 && b2 <= a2) {
    std::cout << "2\n";
  } else {
    std::cout << "?\n";
  }
}

int main() {
  int a1,a2,b1,b2;
  std::cin >> a1 >> a2 >> b1 >> b2;
  Intervals(a1, a2, b1, b2);
  return 0;
}