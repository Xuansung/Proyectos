#include "coche.h"

int main() {
  Car car1("Seat Arona", "BFU312", 31231), car2("Renault Clio", "BWUDF3", 512342);
  std::cout << car1 << car2;
  if (car1 < car2) {
    std::cout << "El coche " << car1.GetModel() << " tiene más kilometraje que " << car2.GetModel() << std::endl;
  } else {
    std::cout << "El coche " << car2.GetModel() << " tiene más kilometraje que " << car1.GetModel() << std::endl;
  }
  return 0;
}