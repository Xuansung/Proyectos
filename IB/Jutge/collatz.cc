#include <iostream>

void Collatz(int number) {
  int it = 0;
  while(number != 1){
    if ( number % 2 == 0) {
      number = number / 2;
    } else {
      number = (number * 3) + 1;
    }
    it++;
  }
  std::cout << it << std::endl; 
}
int main() {
  int number;
  while(std::cin >> number){
    Collatz(number);
  }
  return 0;
}