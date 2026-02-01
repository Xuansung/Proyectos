#ifndef BUSQUEDAS_H
#define BUSQUEDAS_H

#include "grafo.h"
#include <unordered_set>
#include <queue>
#include <stack>
#include <algorithm>
#include <unordered_map>
#include <set>

struct ResultadoBusqueda {
  bool encontrado = false;
  std::vector<int> camino_0based; // incluye origen y destino (0-based)
  double coste = 0.0;

  // Trazas por iteración (comienza en 1):
  // En cada iteración guardamos los conjuntos ACUMULADOS de generados e inspeccionados.
  std::vector<std::vector<int>> generados_iter;     // 1-based para presentar
  std::vector<std::vector<int>> inspeccionados_iter;// 1-based para presentar
};

// Ejecuta BFS (en amplitud) o DFS (en profundidad) iterativo.
ResultadoBusqueda BFS(const Grafo& grafo, int origen, int destino);
ResultadoBusqueda DFS(const Grafo& grafo, int origen,int destino);

void escribirInforme(const std::string& rutaSalida, const std::string& algoritmo, const Grafo& grafo, int origen_1based, int destino_1based,
                     const ResultadoBusqueda& Resultado);

                     
#endif // BUSQUEDAS_H
