#include <iostream>
#include <vector>

std::vector<int>ChangeMoney(const int& count) {
  std::vector<int> coins{200, 100, 50, 20, 10, 5, 2, 1};
  std::vector<int> send;
  int sum = 0;
  while (sum != count) {
    for (int i = 0; i < coins.size(); i++) {
      if (coins[i] + sum <= count) {
        send.push_back(coins[i]);
        sum += coins[i];
        i--;
      }
    }
  }
  return send;
}

int main(int argc, char* argv[]) {
  int input = std::stoi(argv[1]);
  if (argc < 1 || argc > 2) {
    std::cerr << "Error, se debe de añadir un número natural para el cambio." << std::endl;
    return 1;
  }
  std::vector<int> result =ChangeMoney(input);
  for (int i = 0; i < result.size(); i++) {
    if ( i != result.size() - 1)
      std::cout << result[i] << " ";
    else {
      std::cout << result[i] << std::endl;
    }
  }
  return 0;
}