#include "pointset.h"
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
  if (argc < 2 || argc > 3) {
    std::cerr << "Uso: " << argv[0] << " <archivo_entrada> [-d]" << std::endl;
    return 1;
  }
  std::ifstream archivo_entrante(argv[1]);
  std::ofstream archivo_salida("output.txt");
  if (!archivo_entrante) {
    std::cerr << "Error: no se pudo abrir el archivo entrante. " << std::endl;
    return 1;
  }
  int num_puntos;
  archivo_entrante >> num_puntos; 
  CyA::point_vector puntos;
  for (int i = 0; i < num_puntos; i++) {
    CyA::point punto;
    archivo_entrante >> punto;
    puntos.push_back(punto);
  }
  
  CyA::point_set point_set(puntos);
  point_set.quickHull();
  point_set.write_hull(archivo_salida);
  if(argc == 3){
    if (std::string(argv[1]) == "-d" || std::string(argv[2]) == "-d") {
      std::ofstream output_dot{"output.dot"};
      point_set.write(output_dot);
    } else {
      std::cerr << "Uso: " << argv[0] << " <archivo_entrada> [-d]" << std::endl;
      return 1;
    }
  }
  std::cout << point_set.get_max_profundidad() << std::endl;
  return 0;
}