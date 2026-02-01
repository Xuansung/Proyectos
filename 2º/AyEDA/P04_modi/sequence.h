/*
* Autor: Xuan Sun
* Fecha: 22/03/2025
* Resumen: Contiene la clase genérica Sequence y sus clases derivadas DynamicSequence y StaticSequence.
*/
//Headers
#ifndef SEQUENCE_H
#define SEQUENCE_H

//Librerías
#include <vector>
#include <list>

/// @brief Clase base de Sequence que permite identificar una tabla hash de dispersión abierta o cerrada.
/// @tparam Key 
template<class Key>
class Sequence {
 public:
  // Método abstracto para buscar el valor de la clave.
  virtual bool search(const Key& key) const = 0;
  // Método abstracto para insertar la clave.
  virtual bool insert(const Key& key) = 0;
  // Destructor por defecto.
  ~Sequence() {};
};

/// @brief Clase derivada de Sequence de Secuencia Dinámica, por lo que no tiene límite en el tamaño de cada posición.
/// @tparam Key 
template<class Key>
class DynamicSequence : public Sequence<Key> {
 public:
  // Constructor
  DynamicSequence() {}
  // Método para buscar el valor de la clave.
  bool search(const Key& key) const override {
    for (auto& finder : data_) {
      if ( finder == key) 
        return true;
    }
    return false;
  }
  // Método para insertar la clave.
  bool insert(const Key& key) override {
  if(search(key))
    return false;
  data_.push_back(key);
  return true;
  }
 private:
  std::list<Key> data_;
};

/// @brief 
/// @tparam Key 
template<class Key>
class StaticSequence : public Sequence<Key> {
 public:
  // Constructor
  explicit StaticSequence(unsigned BlockSize = 0) : BlockSize_(BlockSize) {
    data_.reserve(BlockSize);
  }
  // Método para buscar el valor de la clave.
  bool search(const Key& key) const override {
  for (auto& finder : data_) {
    if ( finder == key) 
      return true;
  }
  return false;
  }
  // Método para insertar la clave.
  bool insert(const Key& key) override {
    if (isFull() || search(key)) {
      return false;
    }
  data_.push_back(key);
  return true;
  }
  // Método para comprobar si la posición está llena.
  bool isFull() const {
    return data_.size() >= BlockSize_;
  }
 private:
  std::vector<Key> data_;
  unsigned BlockSize_;
};


#endif //SEQUENCE_H