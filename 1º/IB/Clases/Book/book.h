#pragma once

#include <iostream>
#include <string>
#include <iomanip>

class Book {
 public:
  Book(const std::string& name, const int& year, const double& price);  
  void CalculateTaxes(const double& percentage);
  std::string getName() const;
  int getYear() const;
  double getPrice() const;
  double getAfterTaxes() const;
  bool operator<(const Book& other);
 private:
  std::string name_;
  int year_;
  double price_;
  double price_after_taxes_;
};

std::ostream& operator<<(std::ostream& out, const Book& book);