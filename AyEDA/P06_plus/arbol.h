/*
 * Archivo: arbol.h
 * 
 * Descripción:
 *   Definición e implementación de:
 *     - AB (Árbol Binario Abstracto)
 *     - ABB (Árbol Binario de Búsqueda)
 *     - AVL (Árbol Binario de Búsqueda Auto-balanceado)
 *   Soporta inserción, búsqueda, recorrido inorden, impresión por niveles y eliminación de claves
 *   (eliminación sin re‑balanceo en ABB; AVL reutiliza la eliminación de ABB).
 * 
 * Autor original: Xuan Sun
 * Fecha original: 27/04/2025
 * Modificado: 11/05/2025 – Añadida función eliminar()
 * 
 */

#ifndef ARBOL_H
#define ARBOL_H

// Librerías
#include <iostream>
#include <queue>
#include "nodo.h"

// =====================================================
// Clase base abstracta AB (árbol binario genérico)
// =====================================================

template <class Key>
class AB {
 protected:
  NodoB<Key>* raiz;   // Raíz del árbol
  bool traza;         // Habilitar traza
  // Método virtual puro para impresión por niveles
  virtual void printLevels(std::ostream& os) const = 0;

 public:
  // Constructor por defecto
  AB(bool traza = false) : raiz(nullptr), traza(traza) {}
  // Destructor
  virtual ~AB() { clear(raiz); }

  // ------------ Interfaz pública -------------
  virtual bool insertar(const Key& key) = 0;     // Inserción
  virtual bool buscar(const Key& key) const = 0; // Búsqueda
  virtual bool eliminar(const Key& key) = 0;     // *** NUEVO: Eliminación ***
  virtual void inorden(std::ostream& os = std::cout) const = 0; // Recorrido inorden

  // Sobrecarga del operador << para impresión por niveles
  friend std::ostream& operator<<(std::ostream& os, const AB<Key>& arbol) {
    arbol.printLevels(os);
    return os;
  }

 private:
  // Elimina recursivamente todos los nodos (utilizado por el destructor)
  void clear(NodoB<Key>* nodo) {
    if (!nodo) return;
    clear(nodo->izdo);
    clear(nodo->dcho);
    delete nodo;
  }
};

// =====================================================
// Clase ABB (árbol binario de búsqueda)
// =====================================================

template <class Key>
class ABB : public AB<Key> {
 public:
  ABB(bool traza = false) : AB<Key>(traza) {}
  virtual ~ABB() {}

  // ---------------- Inserción -----------------
  virtual bool insertar(const Key& key) override {
    bool inserted = false;
    this->raiz = insertarR(this->raiz, key, inserted);
    return inserted;
  }

  // ---------------- Búsqueda ------------------
  virtual bool buscar(const Key& key) const override {
    NodoB<Key>* actual = this->raiz;
    while (actual) {
      if (key == actual->dato) {
        return true;
      } else if (key < actual->dato) {
        actual = actual->izdo;
      } else {
        actual = actual->dcho;
      }
    }
    return false;
  }

  // ---------------- Eliminación ---------------
  virtual bool eliminar(const Key& key) override {
    bool removed = false;
    this->raiz = eliminarR(this->raiz, key, removed);
    return removed;
  }

  // -------------- Recorrido inorden -----------
  virtual void inorden(std::ostream& os = std::cout) const override {
    inordenR(os, this->raiz);
    os << std::endl;
  }

 protected:
  // ----------- Impresión por niveles ----------
  virtual void printLevels(std::ostream& os) const override {
    if (!this->raiz) {
      os << "Árbol vacío" << std::endl;
      os << "Nivel 0: [. ]" << std::endl;
      return;
    }
    std::queue<NodoB<Key>*> q;
    q.push(this->raiz);
    int level = 0;
    while (!q.empty()) {
      int levelCount = q.size();
      os << "Nivel " << level++ << ": ";
      for (int i = 0; i < levelCount; ++i) {
        NodoB<Key>* nodo = q.front();
        q.pop();
        if (nodo) {
          os << "[" << nodo->dato << "]";
          q.push(nodo->izdo);
          q.push(nodo->dcho);
        } else {
          os << "[.]";
        }
        if (i < levelCount - 1) os << " ";
      }
      os << std::endl;
    }
  }

 private:
  // --------- Inserción recursiva --------------
  NodoB<Key>* insertarR(NodoB<Key>* nodo, const Key& key, bool& inserted) {
    if (!nodo) {
      nodo = new NodoB<Key>(key);
      inserted = true;
    } else if (key < nodo->dato) {
      nodo->izdo = insertarR(nodo->izdo, key, inserted);
    } else if (key > nodo->dato) {
      nodo->dcho = insertarR(nodo->dcho, key, inserted);
    } else {
      inserted = false; // clave duplicada
    }
    return nodo;
  }

  // --------- Eliminación recursiva ------------
  NodoB<Key>* eliminarR(NodoB<Key>* nodo, const Key& key, bool& removed) {
    if (!nodo) return nullptr; // clave no encontrada

    if (key < nodo->dato) {
      nodo->izdo = eliminarR(nodo->izdo, key, removed);
    } else if (key > nodo->dato) {
      nodo->dcho = eliminarR(nodo->dcho, key, removed);
    } else { // nodo encontrado
      removed = true;
      // Caso 1: un solo hijo o ninguno
      if (!nodo->izdo) {
        NodoB<Key>* tmp = nodo->dcho;
        delete nodo;
        return tmp;
      } else if (!nodo->dcho) {
        NodoB<Key>* tmp = nodo->izdo;
        delete nodo;
        return tmp;
      }
      // Caso 2: dos hijos → sustituir por el mínimo del subárbol derecho
      Key minKey;
      nodo->dcho = removeMin(nodo->dcho, minKey);
      nodo->dato = minKey;
    }
    return nodo;
  }

  // Extrae el nodo con la clave mínima del subárbol "nodo" y devuelve el
  // nuevo subárbol tras la eliminación, guardando la clave en minKey.
  NodoB<Key>* removeMin(NodoB<Key>* nodo, Key& minKey) {
    if (!nodo->izdo) {
      minKey = nodo->dato;
      NodoB<Key>* tmp = nodo->dcho;
      delete nodo;
      return tmp;
    }
    nodo->izdo = removeMin(nodo->izdo, minKey);
    return nodo;
  }

  // --------- Recorrido inorden auxiliar -------
  void inordenR(std::ostream& os, NodoB<Key>* nodo) const {
    if (!nodo) return;
    inordenR(os, nodo->izdo);
    os << nodo->dato << " ";
    inordenR(os, nodo->dcho);
  }
};

// =====================================================
// Clase AVL (árbol binario de búsqueda auto‑balanceado)
// =====================================================

template <class Key>
class AVL : public ABB<Key> {
 public:
  AVL(bool traza = false) : ABB<Key>(traza) {}
  virtual ~AVL() {}

  // Inserción con re‑balanceo AVL
  virtual bool insertar(const Key& key) override {
    bool grew = false;
    bool inserted = false;
    this->raiz = insertarAVL(this->raiz, key, grew, inserted);
    return inserted;
  }
  // *** Eliminación heredada de ABB (sin re‑balanceo). ***
  //     Para mantener las garantías AVL sería necesario re‑balancear
  //     tras eliminar. Se puede extender siguiendo un esquema simétrico
  //     al de la inserción si se requiere.

 protected:
  // ---------- Impresión por niveles ------------
  virtual void printLevels(std::ostream& os) const override {
    if (!this->raiz) {
      os << "Árbol vacío" << std::endl;
      os << "Nivel 0: [. ]" << std::endl;
      return;
    }
    std::queue<NodoB<Key>*> q;
    q.push(this->raiz);
    int level = 0;
    while (!q.empty()) {
      int levelCount = q.size();
      os << "Nivel " << level++ << ": ";
      for (int i = 0; i < levelCount; ++i) {
        NodoB<Key>* nodo = q.front();
        q.pop();
        if (nodo) {
          NodoAVL<Key>* nodoAVL = static_cast<NodoAVL<Key>*>(nodo);
          os << "[" << nodoAVL->dato;
          if (this->traza) {
            os << "(" << nodoAVL->bal << ")";
          }
          os << "]";
          q.push(nodoAVL->izdo);
          q.push(nodoAVL->dcho);
        } else {
          os << "[.]";
        }
        if (i < levelCount - 1) os << " ";
      }
      os << std::endl;
    }
  }

 private:
  // --------- Inserción recursiva AVL ----------
  NodoB<Key>* insertarAVL(NodoB<Key>* nodo, const Key& key, bool& grew, bool& inserted) {
    if (!nodo) {
      nodo = new NodoAVL<Key>(key);
      grew = true;
      inserted = true;
      return nodo;
    }
    NodoAVL<Key>* nodoAVL = static_cast<NodoAVL<Key>*>(nodo);
    if (key < nodoAVL->dato) {
      nodoAVL->izdo = insertarAVL(nodoAVL->izdo, key, grew, inserted);
      if (grew) {
        nodoAVL->bal += 1;
        if (nodoAVL->bal == 2) {
          NodoAVL<Key>* nodoIzq = static_cast<NodoAVL<Key>*>(nodoAVL->izdo);
          if (nodoIzq->bal >= 0) {
            if (this->traza) {
              std::cout << "Desbalanceo:\n";
              this->printLevels(std::cout);
              std::cout << "Rotación II en [" << nodoAVL->dato << "(" << nodoAVL->bal << ")]\n";
            }
            nodoAVL = rotateRight(nodoAVL);
          } else {
            if (this->traza) {
              std::cout << "Desbalanceo:\n";
              this->printLevels(std::cout);
              std::cout << "Rotación ID en [" << nodoAVL->dato << "(" << nodoAVL->bal << ")]\n";
            }
            nodoAVL = rotateLeftRight(nodoAVL);
          }
          grew = false;
        } else if (nodoAVL->bal == 0) {
          grew = false;
        }
      }
    } else if (key > nodoAVL->dato) {
      nodoAVL->dcho = insertarAVL(nodoAVL->dcho, key, grew, inserted);
      if (grew) {
        nodoAVL->bal -= 1;
        if (nodoAVL->bal == -2) {
          NodoAVL<Key>* nodoDer = static_cast<NodoAVL<Key>*>(nodoAVL->dcho);
          if (nodoDer->bal <= 0) {
            if (this->traza) {
              std::cout << "Desbalanceo:\n";
              this->printLevels(std::cout);
              std::cout << "Rotación DD en [" << nodoAVL->dato << "(" << nodoAVL->bal << ")]\n";
            }
            nodoAVL = rotateLeft(nodoAVL);
          } else {
            if (this->traza) {
              std::cout << "Desbalanceo:\n";
              this->printLevels(std::cout);
              std::cout << "Rotación DI en [" << nodoAVL->dato << "(" << nodoAVL->bal << ")]\n";
            }
            nodoAVL = rotateRightLeft(nodoAVL);
          }
          grew = false;
        } else if (nodoAVL->bal == 0) {
          grew = false;
        }
      }
    } else {
      // Clave duplicada
      inserted = false;
      grew = false;
      return nodoAVL;
    }
    return nodoAVL;
  }

  // --------- Rotaciones -----------------------
  NodoAVL<Key>* rotateRight(NodoAVL<Key>* A) {
    NodoAVL<Key>* B = static_cast<NodoAVL<Key>*>(A->izdo);
    A->izdo = B->dcho;
    B->dcho = A;
    if (B->bal == 1) {
      A->bal = 0;
      B->bal = 0;
    } else if (B->bal == 0) {
      A->bal = 1;
      B->bal = -1;
    }
    return B;
  }
  NodoAVL<Key>* rotateLeft(NodoAVL<Key>* A) {
    NodoAVL<Key>* C = static_cast<NodoAVL<Key>*>(A->dcho);
    A->dcho = C->izdo;
    C->izdo = A;
    if (C->bal == -1) {
      A->bal = 0;
      C->bal = 0;
    } else if (C->bal == 0) {
      A->bal = -1;
      C->bal = 1;
    }
    return C;
  }
  NodoAVL<Key>* rotateLeftRight(NodoAVL<Key>* A) {
    NodoAVL<Key>* B = static_cast<NodoAVL<Key>*>(A->izdo);
    NodoAVL<Key>* C = static_cast<NodoAVL<Key>*>(B->dcho);
    B->dcho = C->izdo;
    C->izdo = B;
    A->izdo = C->dcho;
    C->dcho = A;
    if (C->bal == -1) B->bal = 1; else B->bal = 0;
    if (C->bal == 1) A->bal = -1; else A->bal = 0;
    C->bal = 0;
    return C;
  }
  NodoAVL<Key>* rotateRightLeft(NodoAVL<Key>* A) {
    NodoAVL<Key>* C = static_cast<NodoAVL<Key>*>(A->dcho);
    NodoAVL<Key>* B = static_cast<NodoAVL<Key>*>(C->izdo);
    C->izdo = B->dcho;
    B->dcho = C;
    A->dcho = B->izdo;
    B->izdo = A;
    if (B->bal == 1) A->bal = -1; else A->bal = 0;
    if (B->bal == -1) C->bal = 1; else C->bal = 0;
    B->bal = 0;
    return B;
  }
};

#endif // ARBOL_H
