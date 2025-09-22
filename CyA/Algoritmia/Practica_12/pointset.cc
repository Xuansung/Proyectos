// Librerías
#include "pointset.h"

namespace CyA {
  // Constructor
  point_set::point_set(const vector<point> &points) : vector<point>(points) , hull_() , max_profundidad_(0) {}
  // Destructor
  point_set::~point_set(void) {}

  void point_set::quickHull(void) {
    hull_.clear(); // Limpia el vector del casco convexo
    CyA::point min_x_point; 
    CyA::point max_x_point;
    // Se obtiene los puntos con la coordenada X mínima y máxima
    x_bounds(min_x_point, max_x_point);
    // Llama a la función recursiva quickHull para los dos lados de la línea base (min,max)
    quickHull(CyA::line(min_x_point, max_x_point), side::LEFT);
    quickHull(CyA::line(min_x_point, max_x_point), side::RIGHT);
    // Remove duplicates
    std::sort(hull_.begin(), hull_.end());
    hull_.erase(std::unique(hull_.begin(), hull_.end()), hull_.end());
  }

  void point_set::quickHull(const CyA::line &l, int side) {
    point farthest;
    // Busca el punto más lejano a la línea 'l' en el lado 'side' (izquiedo o derecho)
    if (farthest_point(l, side, farthest)) {
      // Si se encuentra un punto más lejano, se divide el problema en dos nuevos subproblemas
      // Subproblema 1: línea desde l.first hasta el punto más lejano.
      // Subproblema 2: línea desde  el punto más lejano hasta l.second
      // El lado a buscar se determina negando el resultado de find_side con respecto al otro punto
      max_profundidad_ += 1;
      quickHull(CyA::line(l.first, farthest), -find_side(CyA::line(l.first, farthest), l.second));
      quickHull(CyA::line(farthest, l.second), -find_side(CyA::line(farthest, l.second), l.first));
    } else {
      // Si no hay ningún punto más lejano en ese lado, la línea l es parte del casco convexo.
      hull_.push_back(l.first);
      hull_.push_back(l.second);
    }
  }
  bool point_set::farthest_point(const CyA::line &l, int side, CyA::point &farthest) const {
    farthest = CyA::point_vector::at(0); // Inicializa con el primer punto del vector base
    double max_dist = 0;
    bool found = false;
    // Recorre todos los puntos del conjunto
    for (const CyA::point &p : *this) {
      const double dist = distance(l, p); // Calcula distancia del punto a la línea
      // Verifica si el punto p está en el lado indicado y si es el más lejano hasta ahora
      if (find_side(l, p) == side && dist > max_dist) {
        farthest = p;
        max_dist = dist;
        found = true;
      }
    }
    return found; // Devuelve true si encontró al menos un punto más lejano en el lado especificado
  }

  double point_set::point_2_line(const CyA::line &l, const CyA::point &p) const {
    const CyA::point &p1 = l.first;
    const CyA::point &p2 = l.second;
    // Fórmula del producto cruzado ( determinante) que indica la posición de p con respecto a la línea p1-p2
    return (p.second - p1.second) * (p2.first - p1.first) - (p2.second - p1.second) * (p.first - p1.first);
  }
  double point_set::distance(const CyA::line &l, const CyA::point &p) const {
    return fabs(point_2_line(l, p));
  }


  int point_set::find_side(const line &l, const point &p) const {
    double val = point_2_line(l,p);

    if (val > 0) return side::LEFT;
    if (val < 0) return side::RIGHT;
    return side::CENTER;
  } 

  void point_set::x_bounds(point &min_x, point &max_x) const {
    if (this->empty()) return; // Si no hay puntos, no hace nada-
    min_x = max_x = this->at(0);
    // Recorre todos los puntos para encontrar el de menor x  el de mayor x
    for (const auto &p : *this) {
      if (p.first < min_x.first) {
        min_x = p;
      } else if (p.first > max_x.first) {
        max_x = p;
      }
    }
  }

  void point_set::write_hull(std::ostream &os) const {
    for (const auto &p : hull_) {
      os << "( " << p.first << ", " << p.second << ")" << std::endl;
    }
    os << std::endl;
  }

  void point_set::write(std::ostream &os) const {
    os << "graph {\n";
    for ( int i = 0 ; i < this->size(); i++) {
      os << " node" << i << " [pos=\""<< this->at(i).first << "," << this->at(i).second << "!\"];" << std::endl;
    }
    // Si el casco es vacío o tiene menos de 2 puntos, no podemos formar un polígono
    if (hull_.size() < 2) {
      os << "}\n";
      return;
    }
    // Calcular el centroide del casco
    double cx = 0.0, cy = 0.0;
    for (auto &p : hull_) {
      cx += p.first;
      cy += p.second;
    }
    cx = cx / (double)hull_.size();
    cy = cy / (double)hull_.size();

    // Ordenar los puntos por ángulo con respecto al centroide
    std::vector<point> ordered_hull = hull_;
    std::sort(ordered_hull.begin(), ordered_hull.end(), [cx, cy](const point &a, const point &b) {
      double angA = atan2(a.second - cy, a.first - cx);
      double angB = atan2(b.second - cy, b.first - cx);
      return angA < angB;
    });

    // Obtener los índices originales de los puntos del hull ya ordenados
    std::vector<int> hull_indices;
    for (const auto &hp : ordered_hull) {
      for (int i = 0; i < (int)this->size(); i++) {
        if (this->at(i) == hp) {
          hull_indices.push_back(i);
          break;
        }
      }
    }
    // Conectar los puntos en orden, formando el polígono cerrado
    for (size_t i = 0; i < hull_indices.size(); i++) {
      int current = hull_indices[i]; 
      int next = hull_indices[(i + 1) % hull_indices.size()]; 
      os << "  node" << current << " -- node" << next << ";" << std::endl;
    }
    os << "}\n";
  }

}