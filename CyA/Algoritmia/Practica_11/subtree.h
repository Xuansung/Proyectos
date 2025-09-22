// Header
#pragma once
//Librerías
#include <iostream>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <utility>
#include "point_types.h"

namespace EMST {
  class sub_tree { // Clase tree
   private: // Atributos
     CyA::tree arcs_;
     CyA::point_collection points_;
     double cost_;

   public: // Métodos
     sub_tree(void);
     ~sub_tree(void);
     void add_arc(const CyA::arc &a);
     void add_point(const CyA::point &p);
     bool contains(const CyA::point &p) const;
     void merge(const sub_tree &st, const CyA::weigthed_arc &a);

     inline const CyA::tree& get_arcs(void) const { return arcs_; }
     inline double get_cost(void) const { return cost_; }
  };
  typedef std::vector<sub_tree> sub_tree_vector;
}