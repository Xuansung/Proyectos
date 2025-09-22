/*
 * Archivo: nodo.h
 * 
 * Descripción:
 *   Definición de:
 *     - NodoB (Nodo para el Árbol Binario)
 *     - NodoAVL (Nodo para el Árbol Binario de Búsqueda Auto-balanceado)
 * Autor: Xuan Sun
 * Fecha: 27/04/2025
 */

// Clase Nodo básico para un árbol binario
template <class Key>
class NodoB {
 public:
  Key dato; // Dato almacenado en el nodo
  NodoB<Key>* izdo; // Puntero al hijo izquierdo
  NodoB<Key>* dcho; // Puntero al hijo derecho
  // Constructor por defecto
  NodoB(const Key& dat, NodoB<Key>* i = nullptr, NodoB<Key>* d = nullptr) : dato(dat), izdo(i), dcho(d) {}
  // Destructor por defecto
  virtual ~NodoB() {}
};
// Clase Nodo especializado para árboles AVL
template <class Key>
class NodoAVL : public NodoB<Key> {
 public:
  int bal;  // factor de balance
  // Constructor por defecto
  NodoAVL(const Key& dat, NodoB<Key>* i = nullptr, NodoB<Key>* d = nullptr, int balFactor = 0) : NodoB<Key>(dat, i, d), bal(balFactor) {}
  // Destructor por defecto
  virtual ~NodoAVL() {}
};
