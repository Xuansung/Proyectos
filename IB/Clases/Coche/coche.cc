#include "coche.h"

Car::Car(const std::string& model, const std::string& plate, const int& kilometers) : model_(model) , plate_(plate) , kilometers_(kilometers) {}

std::ofstream& operator<<(std::ostream& out, const Car& car) {
  out << "Modelo:\n" << car.GetModel() << "\nPlaca:\n" << car.GetPlate();
  out << "\nKilometraje:\n" << car.GetKilometers() << std::endl;
}

std::ifstream& operator>>(std::istream& in, Car& car) {
  std::cout << "Añade el modelo:\n";
  std::string model;
  in >> model;
  car.SetModel(model);
  std::cout << "Añade la placa:\n";
  std::string plate;
  in >> plate;
  car.SetPlate(plate);
  std::cout << "Añade el kilometraje:\n";
  int kilometers;
  in >> kilometers;
  car.SetKilometers(kilometers);
}

bool operator<(const Car& car, const Car& other) {
  return car.GetKilometers() < other.GetKilometers();
}