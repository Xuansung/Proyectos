/*
* Autor: Xuan Sun
* Fecha: 22/03/2025
* Resumen: Contiene la clase genérica Dispersion Function y sus clases derivadas ModuleDispersion, SumDispersion, PseudoRandomDispersion.
*/
// Headers
#ifndef DISPERSION_H
#define DISPERSION_H
//Librerías
#include <ctime>


/// @brief Clase genérica de DispersionFunction que intenta dispersar los valores de forma uniforme en la tabla Hash.
/// @tparam Key 
template<class Key>
class DispersionFunction {
 public:
  // Método abstracto de sobrecarga del operador () para pasar parámetros.
  virtual unsigned operator() (const Key& key) const = 0;
  // Destructor por defecto.
  virtual ~DispersionFunction() {};
};

/// @brief Clase derivada de DispersionFunction llamada ModuleDispersion, el cual utiliza el módulo de la clave entre 
//         el tamaño de la tabla para obtener la posición en la que va a residir.  
/// @tparam Key 
template<class Key>
class ModuleDispersion : public DispersionFunction<Key> {
 public:
  // Constructor
  explicit ModuleDispersion(unsigned tableSize) : tableSize_(tableSize) {}
  // Sobrecarga del oeperador() para obtener la posición de la dispersión.
  unsigned operator() (const Key& key) const override {
    return static_cast<unsigned>(key % tableSize_);
  }
 private:
  unsigned tableSize_;
};

/// @brief Clase derivada de DispersionFunction llamada SumDispersion, el cual utiliza el módulo de la suma de cada dígito de la clave entre 
//         el tamaño de la tabla para obtener la posición en la que va a residir.  
/// @tparam Key 
template<class Key>
class SumDispersion : public DispersionFunction<Key> {
 public:
 // Constructor
  explicit SumDispersion(unsigned tableSize) : tableSize_(tableSize) {}
  //Sobrecarga del operador() para obtener la posición de la dispersión.
  unsigned operator() (const Key& key) const override {
    unsigned number = static_cast<unsigned>(key);
    unsigned totalsum = 0;
    while (number > 0) {
      totalsum += number % 10;
      number = number / 10;
    }
    return totalsum % tableSize_;
  }
 private:
  unsigned tableSize_;
};

/// @brief Clase derivada de DispersionFunction llamada PseudoRandomDispersion, el cual utiliza el módulo de una clave aleatoria entre 
//         el tamaño de la tabla para obtener la posición en la que va a residir.
/// @tparam Key 
template<class Key>
class PseudoRandomDispersion : public DispersionFunction<Key> {
 public:
  // Constructor
  explicit PseudoRandomDispersion(unsigned tableSize) : tableSize_(tableSize) {}
  // Sobrecarga del oeperador() para obtener la posición de la dispersión.
  unsigned operator() (const Key& key) const override {
    std::srand(static_cast<unsigned>(key));
    return std::rand() % tableSize_;
  }
private:
 unsigned tableSize_;
};

#endif // DISPERSION_H
