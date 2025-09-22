#include "book.h"

Book::Book(const std::string& name, const int& year, const double& price) :
name_(name), year_(year), price_(price), price_after_taxes_(0.0) {}

void Book::CalculateTaxes(const double& percentage) {
  price_after_taxes_ = price_ * percentage / 100 + price_;
}

std::string Book::getName() const{
  return name_;
}

int Book::getYear() const{
  return year_;
}

double Book::getPrice() const{
  return price_;
}

double Book::getAfterTaxes() const{
  return price_after_taxes_;
}
bool Book::operator<(const Book& other) {
  return year_ < other.year_;
}

std::ostream& operator<<(std::ostream& out, const Book& book) {
  out << book.getName() << ", " << book.getYear() << ", ";
  out << std::fixed << std::setprecision(2) << book.getPrice() << ", " << std::fixed << std::setprecision(2) << book.getAfterTaxes() << std::endl;
  return out;
}