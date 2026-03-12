#ifndef GRAFO_H
#define GRAFO_H

#include <vector>
#include <string>
#include <utility>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <limits>

class Grafo {
 public:
  Grafo(int num_vertices);
  // Cada arista: (vecino, coste)
  using Arista = std::pair<int, double>; // Definición del tipo Arista
  // Carga con el formato de la práctica:
  // primera línea: n
  // luego d(i,j) para 1<=i<j<=n, simétrico; -1 si no hay arista; d(i,i)=0 implícito
  // Los vértices del fichero están numerados desde 1. Internamente usaremos 0..n-1
  static Grafo cargarDesdeFichero(const std::string& ruta);

  int getNumVertices() const { return num_vertices_; }
  int getNumAristas() const { return num_aristas_; }          // aristas no dirigidas
  const std::vector<Arista>& vecinos(int u) const { adyacente_[u]; } // vecinos de u (0-based);

 private:
  int num_vertices_ = 0;
  int num_aristas_ = 0; // número de aristas (no dirigida: contamos cada par una vez)
  std::vector<std::vector<Arista>> adyacente_;
};

#endif // GRAFO_H
