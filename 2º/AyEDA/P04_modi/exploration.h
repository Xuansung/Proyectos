/*
* Autor: Xuan Sun
* Fecha: 22/03/2025
* Resumen: Contiene la clase genérica Exploration Function y sus clases derivadas LinealExploration, CuadraticExploration, DoubleDispersion y ReDispersion
*/
// Headers
#ifndef EXPLORATION_H
#define EXPLORATION_H
//Librerías
#include "dispersion.h"


template<class Key>
class ExplorationFunction {
 public:
  // Método abstracto para sobrecargar el operador () para pasar parámetros.
  virtual unsigned operator()(const Key& key, unsigned i) const = 0;
  // Destructor por defecto.
  virtual ~ExplorationFunction() {};
};

/// @brief La función lineal de exploración tiene como posición el número de intentos.
/// @tparam Key 
template<class Key>
class LinealExploration : public ExplorationFunction<Key> {
 public:
  // Sobrecarga del operador () que devuelve la nueva posición.
  unsigned operator() (const Key& key, unsigned i) const override {return i;}
};

/// @brief La función cuadrática de exploración hace el cuadrado del número de intentos. 
/// @tparam Key 
template<class Key>
class CuadraticExploration : public ExplorationFunction<Key> {
 public:
  // Sobrecarga del operador () que devuelve la nueva posición.
  unsigned operator() (const Key& key, unsigned i) const override {return i * i;}
};

/// @brief Vuelve a hacer una función de dispersión pero multiplicandolo por el número de intentos.
/// @tparam Key 
template<class Key>
class DoubleDispersion : public ExplorationFunction<Key> {
 public:
  // Constructor 
  explicit DoubleDispersion(DispersionFunction<Key>& dispersion_function) : dispersion_function_(dispersion_function) {}
  // Sobrecarga del operador () que devuelve la nueva posición.
  unsigned operator() (const Key& key, unsigned i) const override {
    unsigned offset = dispersion_function_(key);
    if (offset == 0)
      offset = 1;
    return i * offset;
  }
 private:
  DispersionFunction<Key>& dispersion_function_;
};

/// @brief Devuelve una posición aleatoria.
/// @tparam Key 
template<class Key>
class ReDispersion : public ExplorationFunction<Key> {
 public:
  // Sobrecarga del operador () que devuelve la nueva posición.
  unsigned operator()(const Key& key, unsigned i) const override {
    if (i == 1) 
      std::srand((unsigned)key);
    return std::rand();
  }
};

#endif // EXPLORATION_H