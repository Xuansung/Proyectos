#include <iostream>
#include <vector>
void count_and_add(int& num, int& sum) {
  int number;
  sum = 0;
  std::vector<int> vector;
  while (std::cin >> number) {
    vector.push_back(number);
    sum += number;
  }
  num = vector.size();

}