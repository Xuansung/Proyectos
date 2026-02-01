#include "grafo.h"
#include "busquedas.h"
#include <iostream>


static void uso(const char* prog) {
  std::cerr
    << "Uso:\n  " << prog
    << " <grafo.txt> <origen> <destino> <algoritmo> <salida.txt>\n\n"
    << "  <origen>, <destino>: indices 1-based\n"
    << "  <algoritmo>: BFS | DFS\n";
}

int main(int argc, char** argv) {
  if (argc != 6) {
    uso(argv[0]);
    return 1;
  }

  const std::string rutaGrafo = argv[1];
  int origen = std::stoi(argv[2]);
  int destino = std::stoi(argv[3]);
  std::string algoritmo = argv[4];
  const std::string rutaSalida = argv[5];

  try {
    Grafo g = Grafo::cargarDesdeFichero(rutaGrafo);

    if (origen < 1 || origen > g.getNumVertices() || destino < 1 || destino > g.getNumAristas()) {
      throw std::runtime_error("origen/destino fuera de rango");
    }
    int s = origen - 1, t = destino - 1;

    ResultadoBusqueda Resultado;
    if (algoritmo == "BFS" || algoritmo == "bfs") {
      Resultado = BFS(g, s, t);
      escribirInforme(rutaSalida, "BFS", g, origen, destino, Resultado);
    } else if (algoritmo == "DFS" || algoritmo == "dfs") {
      Resultado = DFS(g, s, t);
      escribirInforme(rutaSalida, "DFS", g, origen, destino, Resultado);
    } else {
      throw std::runtime_error("Algoritmo no reconocido (usa BFS o DFS)");
    }

    std::cout << "Resultados escritos en: " << rutaSalida << "\n";
      return 0;
  } catch (const std::exception& ex) {
    std::cerr << "Error: " << ex.what() << "\n";
    return 2;
  }
}
