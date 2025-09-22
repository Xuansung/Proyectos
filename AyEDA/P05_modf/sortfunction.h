/**
 * @brief Declara funciones de ordenación que pueden ser utilizadas por distintas clases.
 * Autor: Xuan Sun
 * Fecha: 07/04/2025
 * Este archivo contiene distintas funciones genéricas para la ordenación de 
 * secuencias, tales como Insertion Sort, Shake Sort, QuickSort, HeapSort y ShellSort,
 * así como funciones auxiliares para el intercambio de elementos, la operación 
 * "baja" en un árbol para HeapSort y la ordenación por incrementos en ShellSort.
 */

// Headers
#ifndef SORTFUNCTION_H
#define SORTFUNCTION_H

// Librerías
#include "sequence.h"
#include <iostream>

/**
 * @brief Ordena una secuencia mediante el algoritmo Insertion Sort.
 *
 * @tparam Key Tipo de dato a ordenar.
 * @param sequence Referencia a la secuencia de elementos a ordenar.
 * @param blocksize Tamaño de la secuencia (número de elementos).
 * @param trace Indica si se deben mostrar mensajes de traza durante la ejecución.
 *
 * El algoritmo recorre la secuencia desde el segundo elemento e inserta cada
 * elemento en la posición adecuada de la parte ordenada (a la izquierda).
 * En caso de que trace sea true, se muestra el estado de la secuencia en cada iteración.
 */
template <class Key>
void insertionSort(StaticSequence<Key>& sequence, unsigned blocksize, bool trace) {
  for (int i = 1; i < blocksize; i++) {
    int j = i;
    Key valor = sequence[i];
    while (j > 0 && valor < sequence[j - 1]) {
      sequence[j] = sequence[j - 1];
      j--;
    }
    sequence[j] = valor;

    if (trace) {
      for (size_t k = 0; k < blocksize - 1; ++k) {
        std::cout << sequence[k] << " ";
      }
      std::cout << std::endl;
    }
  }
}

/**
 * @brief Ordena una secuencia mediante el algoritmo Shake Sort (coctel).
 *
 * @tparam Key Tipo de dato a ordenar.
 * @param sequence Referencia a la secuencia de elementos a ordenar.
 * @param blocksize Tamaño de la secuencia (número de elementos).
 * @param trace Indica si se deben mostrar mensajes de traza durante la ejecución.
 *
 * Este método recorre la secuencia hacia ambos sentidos (inicial y final),
 * moviendo elementos mayores a la derecha y menores a la izquierda en cada pasada.
 * En caso de que trace sea true, se muestra el estado de la secuencia en cada iteración.
 */
template <class Key>
void shakeSort(StaticSequence<Key>& sequence, unsigned blocksize, bool trace) {
  unsigned initial = 1;
  unsigned final = blocksize - 1;
  unsigned aux = blocksize;
  while (initial < final) {
    for (int j = final; j >= initial; j--) 
      if (sequence[j] < sequence[j-1]) {
        swap(sequence[j-1], sequence[j]);
        aux = j;
      }  
    initial = aux + 1;
    for (int j = initial; j <= final; j++) 
      if (sequence[j] < sequence[j-1]) {
        swap(sequence[j-1], sequence [j]);
        aux = j;
      }
    final = aux - 1;
    if (trace) {
      for (size_t k = 0; k < blocksize - 1; ++k) {
        std::cout << sequence[k] << " ";
      }
      std::cout << std::endl;
    }
  }
}

/**
 * @brief Inicia la ejecución del algoritmo QuickSort sobre una secuencia.
 *
 * @tparam Key Tipo de dato a ordenar.
 * @param sequence Referencia a la secuencia de elementos a ordenar.
 * @param blocksize Tamaño de la secuencia (número de elementos).
 * @param trace Indica si se deben mostrar mensajes de traza durante la ejecución.
 *
 * Llama a la función recursiva Qsort para realizar la ordenación.
 */
template <class Key>
void quickSort(StaticSequence<Key>& sequence, unsigned blocksize, bool trace) {
  Qsort(sequence, 0, blocksize - 1, blocksize, trace);
}

/**
 * @brief Ordena una secuencia mediante el algoritmo HeapSort.
 *
 * @tparam Key Tipo de dato a ordenar.
 * @param sequence Referencia a la secuencia de elementos a ordenar.
 * @param blocksize Tamaño de la secuencia (número de elementos).
 * @param trace Indica si se deben mostrar mensajes de traza durante la ejecución.
 *
 * HeapSort utiliza una estructura similar a un montículo (heap) para ordenar los elementos.
 * Primero, construye el heap a partir de la secuencia y luego extrae los elementos
 * de mayor valor para colocarlos al final.
 */
template <class Key>
void heapSort(StaticSequence<Key>& sequence, unsigned blocksize, bool trace) {
  for (int i = blocksize/2 - 1; i >= 0; i--)
    baja(i, sequence, blocksize) ;
   for (int i = blocksize - 1; i > 0; i--) {
    swap(sequence[0],sequence[i]);
    baja(0,sequence,i);
    if (trace) {
      for (size_t k = 0; k < blocksize - 1; ++k) {
        std::cout << sequence[k] << " ";
      }
      std::cout << std::endl;
    }
  };
}

/**
 * @brief Ordena una secuencia mediante el algoritmo ShellSort.
 *
 * @tparam Key Tipo de dato a ordenar.
 * @param sequence Referencia a la secuencia de elementos a ordenar.
 * @param blocksize Tamaño de la secuencia (número de elementos).
 * @param alpha Factor de reducción para la distancia en cada pasada.
 * @param trace Indica si se deben mostrar mensajes de traza durante la ejecución.
 *
 * El algoritmo ShellSort reduce la distancia (gap) usada en cada paso para
 * realizar inserciones parciales. Al final, la distancia llega a 1, donde el
 * algoritmo se asemeja a Insertion Sort. El valor de alpha controla la
 * reducción del gap. En caso de ser inválido, se asigna un valor por defecto (0.4545).
 */
template <class Key>
void shellSort(StaticSequence<Key>& sequence, unsigned blocksize, double alpha, bool trace) {
  if (alpha <= 0.0 || alpha >= 1.0) {
    alpha = 0.4545;  // garantizar un alfa válido (por defecto 0.5 si fuera incorrecto)
  }
  size_t delta = blocksize;
  // Reducir el delta en cada iteración hasta llegar a 1
  while (delta > 1) {
    delta = delta * alpha;
    if (delta < 1)
      delta = 1;
    deltasort(delta, sequence, blocksize, trace);
    if (trace) {
      for (size_t k = 0; k < blocksize - 1; ++k) {
        std::cout << sequence[k] << " ";
      }
      std::cout << std::endl;
    }
    if (delta == 1) 
      break;  // Cuando el delta es 1, la siguiente iteración completaría la ordenación final
  }
}


// Funciones auxiliares
/**
 * @brief Intercambia dos elementos.
 *
 * @tparam Key Tipo de los elementos a intercambiar.
 * @param a Referencia al primer elemento.
 * @param b Referencia al segundo elemento.
 */
template <class Key>
void swap(Key& a, Key& b) {
  Key c = a;
  a = b;
  b = c;
}
/**
 * @brief Operación auxiliar utilizada en HeapSort para restaurar la propiedad de heap.
 *
 * @tparam Key Tipo de dato de la secuencia.
 * @param i Índice del elemento sobre el que se aplica la operación.
 * @param sequence Referencia a la secuencia.
 * @param n Límite superior de la zona en la que se aplica la operación baja.
 *
 * La función ajusta la posición del elemento en índice @p i llevándolo
 * hacia la posición correcta en el heap.
 */
template <class Key>
void baja( int i , StaticSequence<Key>& sequence , int n ) {
  int h;
  while ( 2*i + 1 < n ){
    int h1 = 2*i + 1; 
    int h2 = h1 + 1 ;
    // Seleccionar el hijo mayor (si existe)
    if (h2 < n && sequence[h2] > sequence[h1])
      h = h2;
    else
      h = h1;
    if (sequence[h] <= sequence[i])
      break; 
    else {
      swap(sequence[i],sequence[h]) ;
      i = h;
    }
  }
};

/**
 * @brief Función auxiliar para realizar inserciones parciales en ShellSort.
 *
 * @tparam Key Tipo de dato de la secuencia.
 * @param delta Distancia o "gap" utilizado en la inserción.
 * @param sequence Referencia a la secuencia a ordenar.
 * @param blocksize Tamaño de la secuencia.
 * @param trace Indica si se deben mostrar mensajes de traza durante la ejecución.
 */
template<class Key>
void deltasort(int delta, StaticSequence<Key>& sequence, unsigned blocksize, bool trace) {
  for (int i = delta; i < blocksize; i++) {
    Key valor = sequence[i];
    int j = i;
    while ((j >= delta) && (valor < sequence[j - delta])) {
      sequence[j] = sequence[j - delta];
      j = j - delta;
    }
  sequence[j] = valor;
  }
}

/**
 * @brief Función recursiva principal para el algoritmo QuickSort.
 *
 * @tparam Key Tipo de dato a ordenar.
 * @param sequence Referencia a la secuencia de elementos.
 * @param initial Índice inicial de la subsecuencia a ordenar.
 * @param final Índice final de la subsecuencia a ordenar.
 * @param blocksize Tamaño total de la secuencia.
 * @param trace Indica si se deben mostrar mensajes de traza durante la ejecución.
 *
 * La función particiona la secuencia en torno a un pivote y llama recursivamente
 * a sí misma para ordenar las subpartes izquierda y derecha.
 */
template<class Key>
void Qsort(StaticSequence<Key>& sequence, int initial, int final, unsigned blocksize, bool trace) {
  if (initial < final) {
    int ini = initial;
    int fin = final;
    Key pivote = sequence[(initial + final) / 2];
    while( ini <= fin) {
      while (ini <= fin && sequence[ini] < pivote) ini++;
      while (ini <= fin && sequence[fin] > pivote) fin--;
      if (ini <= fin) {
        swap(sequence[ini],sequence[fin]);
        ini++;
        fin--;
      }
    }
    if (trace) {
      for (size_t k = 0; k < blocksize; k++) {
        std::cout << sequence[k] << " ";
      }
      std::cout << std::endl;
    }
    if (initial < fin)
      Qsort(sequence, initial, fin, blocksize, trace);
    if (ini < final)
      Qsort(sequence, ini, final, blocksize, trace);
  }
}

#endif // SORTFUNCTION_H