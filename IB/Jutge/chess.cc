#include <iostream>
#include <string>

void Chess( const int r, const int c, const std::string& tablero) {
  int valor = 0;
  for ( int i = 0; i < tablero.size(); i++) {
    valor += tablero[i] - '0';
  }
  std::cout << valor << std::endl;
}
int main() {
  int r, c;
  std::cin >> r >> c;
  std::string tabla, acumulador;
  for ( int i = 0; i < r ; i++) {
    std::cin >> tabla;
    acumulador += tabla;
  }
  Chess(r, c, acumulador);
  return 0;
}