/*Funcionalidad: Programa principal para el uso de Big Numbers
 * Fecha: 24/02/2025
 * Autor: Xuan Sun
*/

// Librerías
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include "BigRational.h"

// Función que procesa dos BigRational<N> leídos desde 'input' 
// y escribe los resultados a 'output'.
template <typename BigRationalT>
void ProcesarRacionales(std::istream& input, std::ostream& output) {
  std::string line;
  // Lee línea N1 = ...
  if (!std::getline(input, line)) return; 
  output << line << "\n";
  {
    auto pos = line.find('=');
    if (pos == std::string::npos) return;
    std::string frac_str = line.substr(pos + 1);
    while (!frac_str.empty() && (frac_str.front() == ' ')) {
      frac_str.erase(frac_str.begin());
    }
    std::stringstream ss(frac_str);
    BigRationalT N1;
    ss >> N1; 
    if (!std::getline(input, line)) return;
    output << line << "\n"; 
    pos = line.find('=');
    if (pos == std::string::npos) return;
    frac_str = line.substr(pos + 1);
    while (!frac_str.empty() && (frac_str.front() == ' ')) {
      frac_str.erase(frac_str.begin());
    }
    std::stringstream ss2(frac_str);
    BigRationalT N2;
    ss2 >> N2;
    // N1 == N2
    output << "N1 == N2: " << std::boolalpha << (N1 == N2) << "\n";
    // N1 < N2
    output << "N1 <  N2: " << (N1 < N2) << "\n";
    // N1 + N2
    output << "N1 + N2: " << (N1 + N2) << "\n";
    // N1 - N2
    output << "N1 - N2: " << (N1 - N2) << "\n";
    // N1 * N2
    output << "N1 * N2: " << (N1 * N2) << "\n";
    // N1 / N2 (comprobamos que N2 != 0)
    if (!(N2 == BigRationalT())) {
      output << "N1 / N2: " << (N1 / N2) << "\n";
    } else {
      output << "N1 / N2: ERROR (división por cero)\n";
    }
  }
}

int main(int argc, char* argv[]) {
  // 1) Comprobar argumentos
  //    Esperamos: ./programa <fichero_entrada> <fichero_salida>
  if (argc < 3) {
    std::cerr << "Uso: " << argv[0] << " <fichero_entrada> <fichero_salida>\n";
    return 1;
  }
  // 2) Abrir ficheros
  std::ifstream fin(argv[1]);
  if (!fin.is_open()) {
    std::cerr << "No se pudo abrir '" << argv[1] << "' para lectura.\n";
    return 1;
  }
  std::ofstream fout(argv[2]);
  if (!fout.is_open()) {
    std::cerr << "No se pudo abrir '" << argv[2] << "' para escritura.\n";
    return 1;
  }
  // 3) Leemos la primera línea
  std::string baseLine;
  if (!std::getline(fin, baseLine)) {
    std::cerr << "Fichero de entrada vacío o formato inválido.\n";
    return 1;
  }
  // Reescribimos esa línea en el fichero de salida
  fout << baseLine << "\n";
  // Buscamos el '='
  auto pos = baseLine.find('=');
  if (pos == std::string::npos) {
    std::cerr << "No se encontró '=' en la línea de base.\n";
    return 1;
  }
  // Extraemos la parte derecha y convertimos a entero
  std::string baseStr = baseLine.substr(pos + 1);
  // Quitamos espacios
  while (!baseStr.empty() && (baseStr.front() == ' ')) {
    baseStr.erase(baseStr.begin());
  }
  int baseValue = std::stoi(baseStr);  // base = 2, 8, 10, 16, ...
  // 4) Según la base, instanciamos la plantilla BigRational<base> y llamamos a ProcesarRacionales
  switch (baseValue) {
    case 2: {
      ProcesarRacionales< BigRational<2> >(fin, fout);
      break;
    }
    case 8: {
      ProcesarRacionales< BigRational<8> >(fin, fout);
      break;
    }
    case 10: {
      ProcesarRacionales< BigRational<10> >(fin, fout);
      break;
    }
    case 16: {
      ProcesarRacionales< BigRational<16> >(fin, fout);
      break;
    }
    default:
      std::cerr << "Base no soportada: " << baseValue << "\n";
      break;
  }
  fin.close();
  fout.close();
  return 0;
}
