#include <iostream>
#include <vector>
#include <algorithm>

void reverse(const int& size) {
  std::vector<int> vector;
  for (int i = 0; i < size ; i++) {
    int number;
    std::cin >> number;
    vector.push_back(number); 
  }
  for (int i = size - 1; i >= 0; i--) {
    if ( i == 0 ){
      std::cout << vector[i];
    } else {
      std::cout << vector[i] << " ";
    }
  }
  std::cout << std::endl;
}

int main() {
  int size;
  while (std::cin >> size) {
    reverse(size);
  }
  return 0;
}