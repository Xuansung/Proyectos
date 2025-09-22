#include "book.h"

int main(int argc, char* argv[]) {
  if ( argc != 2) {
    std::cerr << "Tienes que añadir al menos un porcentaje." << std::endl;
    return 1;
  }
  double percentage = std::stod(argv[1]);
  Book book1("El quijote de la Mancha", 1605, 21.00);
  Book book2("El lazarillo de Tormes", 1554, 29.26);
  Book book3("Los pilares de la Tierra", 1989, 27);
  book1.CalculateTaxes(percentage);
  book2.CalculateTaxes(percentage);
  book3.CalculateTaxes(percentage);
  std::cout << book1 << book2 << book3 << std::endl;
  if (book1 < book2) {
    std::cout << "El libro " << book1.getName() << " es más antiguo que ";
    std::cout << book2.getName() << "." << std::endl;
  } else {
    std::cout << "El libro " << book2.getName() << "es más antiguo que ";
    std::cout << book1.getName() << "." << std::endl;
    
  }
  return 0;
}