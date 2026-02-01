#include "grafo.h"


Grafo::Grafo(int num_vertices) : num_vertices_(num_vertices), num_aristas_(0), adyacente_(num_vertices) {}

Grafo Grafo::cargarDesdeFichero(const std::string& ruta) {
  std::ifstream in(ruta);
  if (!in) throw std::runtime_error("No se pudo abrir el fichero: " + ruta);

  int num_nodos;
  if (!(in >> num_nodos) || num_nodos <= 0) {
    throw std::runtime_error("Primera línea inválida: se esperaba n > 0");
  }

  Grafo grafo(num_nodos);

  // Leemos d(i,j) para 1<=i<j<=n, en ese orden (como en el ejemplo de la práctica)
  // Si hay saltos de línea o comentarios a la derecha, el operador >> ignora espacios.
  for (int i = 1; i <= num_nodos; i++) {
    for (int j = i + 1; j <= num_nodos; j++) {
      double coste;
      if (!(in >> coste)) {
        std::ostringstream oss;
        oss << "Faltan distancias: esperaba d(" << i << "," << j << ")";
        throw std::runtime_error(oss.str());
      }
      if (coste >= 0.0) {
        int nodo1 = i - 1;
        int nodo2 = j - 1;
        grafo.adyacente_[nodo1].push_back({nodo2, coste});
        grafo.adyacente_[nodo2].push_back({nodo1, coste});
        grafo.num_aristas_++;
      }
    }
  }
  return grafo;
}
