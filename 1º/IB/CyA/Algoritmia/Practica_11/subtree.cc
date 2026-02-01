// Librerías
#include "subtree.h"

namespace EMST {
  // Constructor
  sub_tree::sub_tree(void) : arcs_(), points_(), cost_(0) {}
  // Destructor
  sub_tree::~sub_tree(void) {}
  // Funcion añadir arcos
  void sub_tree::add_arc(const CyA::arc &a) {
    arcs_.push_back(a);
    points_.insert(a.first);
    points_.insert(a.second);
  }
  // Funcion añadir puntos
  void sub_tree::add_point(const CyA::point &p) {
    points_.insert(p);
  }
  // Función para encontrar punto
  bool sub_tree::contains(const CyA::point &p) const {
    return points_.find(p) != points_.end();
  }
  // Función ordenar los arcos por costes
  void sub_tree::merge(const sub_tree &st, const CyA::weigthed_arc &a) {
    arcs_.insert(arcs_.end(), st.arcs_.begin(), st.arcs_.end());
    arcs_.push_back(a.second);
    points_.insert(st.points_.begin(), st.points_.end());
    cost_ += a.first + st.get_cost();
  }
}