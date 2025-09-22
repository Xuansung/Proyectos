#include <vector>
#include <iostream>
#include <algorithm>
int main() {
  int number;
  std::vector<int> vector;
  std::cin >> number;
  for (int i = 0; i < number; i++) {
    std::cin >> vector[i];
  }
  std::sort(vector.begin(), vector.end());
  for (int i = 0; i < number; i++) {
    if (vector[i] != vector[i + 1]) {
      
    }
  }
}