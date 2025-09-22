// Header
#pragma once
// Libreria
#include <string>
#include <fstream>
#include <iostream>

class Car {
 public:
  Car(const std::string& model, const std::string& plate, const int& kilometers);
  std::string GetModel() const { return model_;}
  std::string GetPlate() const { return plate_;}
  int GetKilometers() const { return kilometers_;}
  void SetModel(const std::string& model) { model_ = model;}
  void SetPlate(const std::string& plate) { plate_ = plate;}
  void SetKilometers(const int& kilometers) { kilometers_ = kilometers;} 
 private:
  std::string plate_;
  std::string model_;
  int kilometers_;
  
};

std::ofstream& operator<<(std::ostream& out, const Car& car);
std::ifstream& operator>>(std::istream& in, Car& car);
bool operator<(const Car& car, const Car& other);