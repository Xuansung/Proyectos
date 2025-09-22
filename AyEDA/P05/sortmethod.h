/**
 * @brief Contiene la declaración de las clases relacionadas con métodos de ordenación.
 * Autor: Xuan Sun
 * Fecha: 07/04/2025
 * Este archivo define la clase base abstracta SortMethod y las clases derivadas
 * que implementan distintos algoritmos de ordenación (InsertionSort, ShakeSort, 
 * QuickSort, HeapSort y ShellSort).
 */

// Headers
#ifndef SORTMETHOD_H
#define SORTMETHOD_H

//Librerías
#include "sequence.h"
#include "sortfunction.h"

/**
 * @class SortMethod
 * @brief Clase base abstracta para la ejecución de algoritmos de ordenación.
 *
 * @tparam Key Tipo de dato que será ordenado.
 *
 * SortMethod provee la interfaz y componentes necesarios para que 
 * sus clases hijas implementen algoritmos de ordenación específicos.
 */
template<class Key>
class SortMethod {
 public:
  // Constructor
  SortMethod(unsigned blocksize, bool trace) : blockSize_(blocksize), trace_(trace), sequence_(blocksize) {}
  // Método abstracto para ordenar
  virtual void Sort() = 0;
  // Destructor 
  virtual ~SortMethod() {};
  // Método para obtener el atributo privado sequence_
  StaticSequence<Key>& getSequence() {
    return sequence_;
  }
 protected:
  // Número de elementos a ordenar
  unsigned blockSize_;
  // Mostrar o no la traza
  bool trace_;
  // Secuencia de números al que ordenar
  StaticSequence<Key> sequence_;
};

/**
 * @class InsertionSort
 * @brief Implementa el algoritmo de ordenación por inserción.
 *
 * @tparam Key Tipo de dato que será ordenado.
 */
template<class Key>
class InsertionSort : public SortMethod<Key> {
 public:
  // Constructor
  InsertionSort(unsigned blocksize, bool trace) : SortMethod<Key>(blocksize, trace) {}
  // Método de ordenación
  void Sort() override {
    insertionSort(this->sequence_,this->blockSize_, this->trace_);
  }
};

/**
 * @class ShakeSort
 * @brief Implementa el algoritmo de ordenación Shake Sort (coctel).
 *
 * @tparam Key Tipo de dato que será ordenado.
 */
template<class Key>
class ShakeSort : public SortMethod<Key> {
 public:
  // Constructor
  ShakeSort(unsigned blocksize, bool trace) : SortMethod<Key>(blocksize, trace) {}
  // Método de ordenación
  void Sort() override {
    shakeSort(this->sequence_,this->blockSize_, this->trace_);
  }
};

/**
 * @class QuickSort
 * @brief Implementa el algoritmo de ordenación rápida (QuickSort).
 *
 * @tparam Key Tipo de dato que será ordenado.
 */
template<class Key>
class QuickSort : public SortMethod<Key> {
 public:
  // Constructor
  QuickSort(unsigned blocksize, bool trace) : SortMethod<Key>(blocksize, trace) {}
  // Método de ordenación
  void Sort() override {
    quickSort(this->sequence_,this->blockSize_, this->trace_);
  }
};

/**
 * @class HeapSort
 * @brief Implementa el algoritmo de ordenación por montículos (HeapSort).
 *
 * @tparam Key Tipo de dato que será ordenado.
 */
template<class Key>
class HeapSort : public SortMethod<Key> {
 public:
  // Constructor
  HeapSort(unsigned blocksize, bool trace) : SortMethod<Key>(blocksize, trace) {}
  // Método de ordenación
  void Sort() override {
    heapSort(this->sequence_,this->blockSize_, this->trace_);
  }
};

/**
 * @class ShellSort
 * @brief Implementa el algoritmo de ordenación ShellSort.
 *
 * @tparam Key Tipo de dato que será ordenado.
 */
template<class Key>
class ShellSort : public SortMethod<Key> {
 public:
  // Constructor
  ShellSort(unsigned blocksize, bool trace,double alpha) : SortMethod<Key>(blocksize, trace) , alpha_(alpha){
    // Si alpha está fuera de rango se pone default a 0.4545
    if (alpha_ <= 0.0 || alpha_ >= 1.0) {
      alpha_ = 0.4545;
    }
  }
  // Método de ordenación
  void Sort() override {
    shellSort(this->sequence_,this->blockSize_, alpha_, this->trace_);
  }
 private:
  // Atributo que se utiliza para multiplicarlo por delta.
  double alpha_;
};
#endif //SORTMETHOD_H