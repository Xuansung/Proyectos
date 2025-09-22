/*
* Autor: Xuan Sun
* Fecha: 22/03/2025
* Resumen: Contiene la clase genérica HashTable y su clases derivada HashTable de dispersión abierta.
*/
//Librerías
#include <vector>
#include "dispersion.h"
#include "exploration.h"
#include "sequence.h"
//Headers
#ifndef HASH_H
#define HASH_H

template<class Key, class Container>
class HashTable {
 public:
 // Constructor
  HashTable(unsigned tablesize, unsigned blocksize, DispersionFunction<Key>& fd, ExplorationFunction<Key>& fe) : tableSize_(tablesize), blockSize_(blocksize), fd_(fd), fe_(fe) {
    table_.reserve(tableSize_);
    for(unsigned i = 0 ; i < tablesize; i++) {
      table_.emplace_back(blockSize_);
    }
  }
  // Método Insert para insertar la clave
  bool insert(const Key& key) {
    unsigned initIndex = fd_(key) % tableSize_;
    if (table_[initIndex].insert(key)) return true;
    // Si no se insertó ahí, o ya existe o está lleno
    if (!table_[initIndex].isFull()) return false; // ya existía
    // Explorar
    for (unsigned i = 1; i < tableSize_; i++) {
        unsigned offset = fe_(key, i);
        unsigned index = (initIndex + offset) % tableSize_;
        if (table_[index].insert(key)) return true;
        if (!table_[index].isFull()) return false;
    }
    return false;
  }
  // Método Search para buscar la clave
  bool search(const Key& key) const {
    unsigned initIndex = fd_(key) % tableSize_;
    if (table_[initIndex].search(key)) return true;
    if (!table_[initIndex].isFull()) return false;
    // Explorar
    for (unsigned i=1; i<tableSize_; i++) {
        unsigned offset = fe_(key, i);
        unsigned index = (initIndex + offset) % tableSize_;
        if (table_[index].search(key)) return true;
        if (!table_[index].isFull()) return false;
    }
    return false;
  }
 private:
  unsigned tableSize_;
  unsigned blockSize_;
  DispersionFunction<Key>& fd_;
  ExplorationFunction<Key>& fe_;
  std::vector<Container> table_;

};

// Especialización para dispersión abierta
template <class Key>
class HashTable<Key, DynamicSequence<Key>> {
 public:
  HashTable(unsigned tablesize, DispersionFunction<Key>& fd) : tableSize_(tablesize), fd_(fd) {
    table_.resize(tableSize_);
  }
  bool insert(const Key& key) {
    unsigned index = fd_(key) % tableSize_;
    return table_[index].insert(key);
  }
  bool search(const Key& key) const {
    unsigned index = fd_(key) % tableSize_;
    return table_[index].search(key);
  }
 private:
  unsigned tableSize_;
  DispersionFunction<Key>& fd_;
  std::vector<DynamicSequence<Key>> table_;
};
#endif //HASH_H