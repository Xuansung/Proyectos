#include <iostream>

void Intervals1(const int a1, const int a2, const int b1, const int b2) {
  if ( a1 == b1 && a2 == b2) {
    std::cout << "=";
  } else if (a1 >= b1 && a2 <= b2) {
    std::cout << "1";
  } else if ( b1 >= a1 && b2 <= a2) {
    std::cout << "2";
  } else {
    std::cout << "?";
  }
}
void Intervals(const int a1,const int a2, const int b1, const int b2) {
  int c1,c2;
  bool intervalo1 = true,intervalo2 = true;
  if (a1 >= b1 && a1 <= b2) {
    c1 = a1;
  } else if (b1 >= a1 && b1 <= a2) {
    c1 = b1;
  } else {
    intervalo1 = false;
  }
  if (a2 >= b1 && a2 <= b2) {
    c2= a2;
  } else if (b2 >= a1 && b2 <= a2) {
    c2 = b2;
  } else {
    intervalo2 = false;
  }
  if (intervalo1 && intervalo2) 
    std::cout << "[" << c1 << "," << c2 << "]";
  else {
    std::cout << "[]";
  }
}
int main() {
  int a1,a2,b1,b2;
  std::cin >> a1 >> a2 >> b1 >> b2;
  Intervals1(a1, a2, b1, b2);
  std::cout << " , ";
  Intervals(a1, a2, b1, b2);
  std::cout << std::endl;
  return 0;
}