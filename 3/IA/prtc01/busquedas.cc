#include "busquedas.h"


// Utilidad para reconstruir camino (0-based) desde padres
static std::vector<int> reconstruir(int origen, int destino, const std::vector<int>& padre) {
  std::vector<int> camino;
  if (destino < 0) 
    return camino;
  for (int dest = destino; dest != -1; dest = padre[dest]) 
    camino.push_back(dest);
  std::reverse(camino.begin(), camino.end());
  if (camino.empty() || camino.front() != origen) 
    camino.clear();
  return camino;
}

static double costeCamino(const Grafo& grafo, const std::vector<int>& camino) {
  if (camino.size() < 2) 
    return 0.0;
  double coste = 0.0;
  for (int i = 0; i + 1 < camino.size(); i++) {
    int nodo1 = camino[i], nodo2 = camino[i+1];
    bool encontrado = false;
    for (const auto& arista : grafo.vecinos(nodo1)) {
      if (arista.first == nodo2) {
        coste += arista.second;
        encontrado = true;
        break;
      }
    }
    if (!encontrado)
      return -1.0; // Camino no válido
  }
  return coste;
}

static void push_traza(std::set<int>& generados, std::set<int>& inspeccionados, std::vector<std::vector<int>>& generados_iter,
                       std::vector<std::vector<int>>& inspeccionados_iter) {
  // Guardamos copia 1-based ordenada
  std::vector<int> Generados_vec(generados.begin(), generados.end());
  std::vector<int> Inspeccionados_vec(inspeccionados.begin(), inspeccionados.end());
  // (1-based para presentar)
  for (int& i : Generados_vec) 
    i++;
  for (int& i : Inspeccionados_vec)
    i++;
  generados_iter.push_back(Generados_vec);
  inspeccionados_iter.push_back(Inspeccionados_vec);
}

ResultadoBusqueda BFS(const Grafo& grafo, int origen, int destino) {
  ResultadoBusqueda Resultado;
  const int num_vertices = grafo.getNumVertices();
  std::vector<int> padre(num_vertices, -1);
  std::vector<bool> visitado(num_vertices, false);
  std::queue<int> cola;
  std::set<int> generados, inspeccionados; // acumulados (0-based, ordenados)

  visitado[origen] = true;
  cola.push(origen);
  generados.insert(origen);
  push_traza(generados, inspeccionados, Resultado.generados_iter, Resultado.inspeccionados_iter);

  while (!cola.empty()) {
    int nodo = cola.front(); 
      cola.pop();
    // inspeccionamos nodo
    inspeccionados.insert(nodo);
    push_traza(generados, inspeccionados, Resultado.generados_iter, Resultado.inspeccionados_iter);
    if (nodo == destino) {
      Resultado.encontrado = true;
      Resultado.camino_0based = reconstruir(origen, destino, padre);
      Resultado.coste = costeCamino(grafo, Resultado.camino_0based);
      return Resultado;
    }
    for (const auto& arista : grafo.vecinos(nodo)) {
      int vecino = arista.first;
      double coste = arista.second;
      if (!visitado[vecino]) {
        visitado[vecino] = true;
        padre[vecino] = nodo;
        cola.push(vecino);
        generados.insert(vecino);
        // Guardamos la traza tras cada cambio importante
        push_traza(generados, inspeccionados, Resultado.generados_iter, Resultado.inspeccionados_iter);
      }
    }
  }
  // no encontrado
  Resultado.encontrado = false;
  return Resultado;
}

ResultadoBusqueda DFS(const Grafo& grafo, int origen, int destino) {
  ResultadoBusqueda Resultado;
  const int num_vertices = grafo.getNumVertices();
  std::vector<int> padre(num_vertices, -1);
  std::vector<bool> visitado(num_vertices, false);
  std::stack<int> pila;
  std::set<int> generados, inspeccionados;

  pila.push(origen);
  generados.insert(origen);
  push_traza(generados, inspeccionados, Resultado.generados_iter, Resultado.inspeccionados_iter);

  while (!pila.empty()) {
    int nodo = pila.top(); 
    pila.pop();
    if (!visitado[nodo]) {
      visitado[nodo] = true;
      inspeccionados.insert(nodo);
      push_traza(generados, inspeccionados, Resultado.generados_iter, Resultado.inspeccionados_iter);

      if (nodo == destino) {
        Resultado.encontrado = true;
        Resultado.camino_0based = reconstruir(origen, destino, padre);
        if (Resultado.camino_0based.empty()) {
          // En DFS, si sacamos destino sin haberlo enlazado (poco probable), lo fijamos:
          Resultado.camino_0based = {origen, destino};
        }
        Resultado.coste = costeCamino(grafo, Resultado.camino_0based);
        return Resultado;
      }

      // Para DFS iterativo, empujamos vecinos en orden inverso para
      // explorar en el orden natural de la lista.
      const auto& N = grafo.vecinos(nodo);
      for (int i = int(N.size()) - 1; i >= 0; --i) {
        int v = N[i].first;
        if (!visitado[v]) {
          if (padre[v] == -1) 
            padre[v] = nodo;
          pila.push(v);
          generados.insert(v);
          push_traza(generados, inspeccionados, Resultado.generados_iter, Resultado.inspeccionados_iter);
        }
      }
    }
  }
  Resultado.encontrado = false;
  return Resultado;
}

void escribirInforme(const std::string& rutaSalida,
                     const std::string& algoritmo,
                     const Grafo& grafo,
                     int s_1based, int t_1based,
                     const ResultadoBusqueda& Resultado) {
    std::ofstream out(rutaSalida);
    if (!out) throw std::runtime_error("No se pudo abrir el fichero de salida: " + rutaSalida);

    out << "Numero de nodos del grafo: " << grafo.getNumVertices() << "\n";
    out << "Numero de aristas del grafo: " << grafo.getNumAristas() << "\n\n";
    out << "Vertice origen: " << s_1based << "\n";
    out << "Vertice destino: " << t_1based << "\n";
    out << "Algoritmo: " << algoritmo << "\n\n";

    // Iteraciones (comenzamos en 1)
    for (size_t k = 0; k < Resultado.generados_iter.size(); ++k) {
        out << "Iteracion " << (k + 1) << "\n";
        out << "Nodos generados: ";
        if (Resultado.generados_iter[k].empty()) out << "-";
        else {
            for (size_t i = 0; i < Resultado.generados_iter[k].size(); ++i) {
                if (i) out << ", ";
                out << Resultado.generados_iter[k][i];
            }
        }
        out << "\n";
        out << "Nodos inspeccionados: ";
        if (Resultado.inspeccionados_iter[k].empty()) out << "-";
        else {
            for (size_t i = 0; i < Resultado.inspeccionados_iter[k].size(); ++i) {
                if (i) out << ", ";
                out << Resultado.inspeccionados_iter[k][i];
            }
        }
        out << "\n\n";
    }

    if (Resultado.encontrado) {
        out << "Camino: ";
        for (size_t i = 0; i < Resultado.camino_0based.size(); ++i) {
            if (i) out << " - ";
            out << (Resultado.camino_0based[i] + 1);
        }
        out << "\n";
        out << std::fixed << std::setprecision(3);
        out << "Costo: " << Resultado.coste << "\n";
    } else {
        out << "No se encontro un camino entre " << s_1based
            << " y " << t_1based << ".\n";
    }
}
