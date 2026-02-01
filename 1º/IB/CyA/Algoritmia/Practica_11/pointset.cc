// Librerías
#include "pointset.h"
// CONST
#define UMBRAL 25
// Constructor
point_set::point_set(const CyA::point_vector &points) : CyA::point_vector(points), emst_() {}
// Destructor
point_set::~point_set() {}

void point_set::EMST(void) {
  CyA::arc_vector av;
  compute_arc_vector(av);
  forest st;
  for (const CyA::point &p : *this) {
    sub_tree s;
    s.add_point(p);
    st.push_back(s);
  }
  for (const CyA::weigthed_arc &a : av) {
    int i, j;
    find_incident_subtrees(st, a.second, i, j);
    if (i != j) {
      merge_subtrees(st, a.second, i, j);
    }
  }
  emst_ = st[0].get_arcs();
}

void point_set::compute_arc_vector(CyA::arc_vector &av) const {
  av.clear();
  const int n = size();
  for (int i = 0; i < n - 1; ++i) {
    const CyA::point &p_i = (*this)[i];
    for (int j = i + 1; j < n; ++j) {
      const CyA::point &p_j = (*this)[j];
      const double dist = euclidean_distance(std::make_pair(p_i, p_j));
      av.push_back(std::make_pair(dist, std::make_pair(p_i, p_j)));
    }
  }
  std::sort(av.begin(), av.end());
}

void point_set::find_incident_subtrees(const forest& st, const CyA::arc &a, int& i, int& j) const {
  i = -1;
  j = -1;
  for (int k = 0; k < st.size() ; k++) {
    if (st[k].contains(a.first)) {
      i = k;
    }
    if (st[k].contains(a.second)) {
      j = k;
    }
    if (i != -1 && j != -1) {
      break;
    }
  }
}

void point_set::merge_subtrees(forest& st, const CyA::arc &a, int i, int j) const {
  if(euclidean_distance(a) > UMBRAL){
  st[i].merge(st[j], std::make_pair(euclidean_distance(a), a));
  st.erase(st.begin() + j);
  }
}

double point_set::compute_cost(void) const {
  double cost = 0;
  for (const CyA::arc &a : emst_) {
    cost += euclidean_distance(a);
  }
  return cost;
}

double point_set::euclidean_distance(const CyA::arc& a) const {
  double x = a.second.first - a.first.first;
  double y = a.second.second - a.first.second;
  return sqrt( x * x + y * y);
}

void point_set::write_tree(std::ostream &os) const {
  for (const CyA::arc &a : emst_) {
    os << "( " << a.first.first << ", " << a.first.second << ") -> ( ";
    os << a.second.first << ", " << a.second.second << ")" << std::endl;
  }
  os << std::endl;
  os << compute_cost() << std::endl;
}

void point_set::write(std::ostream &os) const {
  os << "graph {" << std::endl;
  // Escribir los nodos con sus posiciones
  for (size_t i = 0; i < this->size(); ++i) {
    const CyA::point& p = (*this)[i];
    os << "  " << i << " [pos=\"" << p.first << "," << p.second << "!\"];" << std::endl;
  }
  // Escribir las aristas del árbol generador mínimo
  for (const CyA::arc& a : emst_) {
    int index1 = std::distance(this->begin(), std::find(this->begin(), this->end(), a.first));
    int index2 = std::distance(this->begin(), std::find(this->begin(), this->end(), a.second));
    os << "  " << index1 << " -- " << index2 << ";" << std::endl;
  }

  os << "}" << std::endl;
}