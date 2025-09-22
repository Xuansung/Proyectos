// Headers
#pragma once
// Librerías
#include <iostream>
#include <utility>
#include <vector>
#include <set>
// CONST
#define MAX_SZ 3
#define MAX_PREC 0

// Tipos de datos
namespace CyA {
  typedef std::pair<double, double> point; // punto
  typedef std::vector<point> point_vector; // vector de puntos
  typedef std::pair<point, point> arc; // arco
  typedef std::pair<double, arc> weigthed_arc; // arco ponderado
  typedef std::vector<weigthed_arc> arc_vector; // vector de arcos ponderados
  typedef std::set<point> point_collection; // colección de puntos
  typedef std::vector<arc> tree; // árbol, vector de arcos
}
// Funciones para la entrada y salida estándar.
std::ostream& operator<<(std::ostream& os, const CyA::point_vector& ps);
std::ostream& operator<<(std::ostream& os, const CyA::point& ps);

std::istream& operator>>(std::istream& is, CyA::point_vector& ps);
std::istream& operator>>(std::istream& is, CyA::point& ps);