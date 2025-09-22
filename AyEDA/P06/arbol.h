/*
 * Archivo: arbol.h
 * 
 * Descripción:
 *   Definición e implementación de:
 *     - AB (Árbol Binario Abstracto)
 *     - ABB (Árbol Binario de Búsqueda)
 *     - AVL (Árbol Binario de Búsqueda Auto-balanceado)
 *   Soporta inserción, búsqueda, recorrido inorden e impresión por niveles,
 *   con opción de traza de balanceo en el caso de AVL.
 * 
 * Autor: Xuan Sun
 * Fecha: 27/04/2025
 * 
 */

// Librerías
#include <iostream>
#include <queue>

// Clase base abstracta AB(árbol binario)
template <class Key>
class AB {
 protected:
  NodoB<Key>* raiz; // Raíz del árbol
  bool traza; // Habilitar traza
  // Método virtual puro para impresión por niveles
  virtual void printLevels(std::ostream& os) const = 0;

 public:
  // Constructor por defecto
  AB(bool traza = false) : raiz(nullptr), traza(traza) {}
  // Destructor por defecto
  virtual ~AB() {
    clear(raiz);
  }
  // Métodos virtuales puros
  virtual bool insertar(const Key& key) = 0;
  virtual bool buscar(const Key& key) const = 0;
  virtual void inorden(std::ostream& os = std::cout) const = 0;
  // Sobrecarga del operador << para impresión por niveles
  friend std::ostream& operator<<(std::ostream& os, const AB<Key>& arbol) {
    arbol.printLevels(os);
    return os;
  }
 private:
  // Método auxiliar para eliminar todos los nodos del árbol.
  void clear(NodoB<Key>* nodo) {
    if(!nodo) return;
    clear(nodo->izdo);
    clear(nodo->dcho);
    delete nodo;
  }
};
// Clase ABB (árbol binario de búsqueda)
template <class Key>
class ABB : public AB<Key> {
 public:
  ABB(bool traza = false) : AB<Key>(traza) {}
  virtual ~ABB() {}
  // Insertar clave en ABB (sin duplicados)
  virtual bool insertar(const Key& key) override {
    bool inserted = false;
    this->raiz = insertarR(this->raiz, key, inserted);
    return inserted;
  }
  // Busca una clave en el ABB
  virtual bool buscar(const Key& key) const override {
    NodoB<Key>* actual = this->raiz;
    while(actual) {
      if(key == actual->dato) {
      return true;
      } else if(key < actual->dato) {
      actual = actual->izdo;
      } else {
      actual = actual->dcho;
      }
    }
    return false;
  }
  // Recorrido inorden del ABB
  virtual void inorden(std::ostream& os = std::cout) const override {
    inordenR(os, this->raiz);
    os << std::endl;
  }
 protected:
  // Imprime el árbol nivel a nivel
  virtual void printLevels(std::ostream& os) const override {
    if(!this->raiz) {
      os << "Árbol vacío" << std::endl;
      os << "Nivel 0: [.]" << std::endl;
      return;
    }
    std::queue<NodoB<Key>*> q;
    q.push(this->raiz);
    int level = 0;
    while(!q.empty()) {
      int levelCount = q.size();
      os << "Nivel " << level++ << ": ";
      for(int i = 0; i < levelCount; ++i) {
        NodoB<Key>* nodo = q.front();
        q.pop();
        if(nodo) {
          os << "[" << nodo->dato << "]";
          q.push(nodo->izdo);
          q.push(nodo->dcho);
        } else {
          os << "[.]";
        // no encolamos hijos de null
        }
      if(i < levelCount - 1) os << " ";
      }
      os << std::endl;
    }
  }
 private:
  // Inserción recursiva
  NodoB<Key>* insertarR(NodoB<Key>* nodo, const Key& key, bool& inserted) {
    if(!nodo) {
      nodo = new NodoB<Key>(key);
      inserted = true;
    } else if(key < nodo->dato) {
      nodo->izdo = insertarR(nodo->izdo, key, inserted);
    } else if(key > nodo->dato) {
      nodo->dcho = insertarR(nodo->dcho, key, inserted);
    } else {
      inserted = false;
    }
    return nodo;
  }
  // Recorrido inorden auxiliar
  void inordenR(std::ostream& os, NodoB<Key>* nodo) const {
    if(!nodo) return;
    inordenR(os, nodo->izdo);
    os << nodo->dato << " ";
    inordenR(os, nodo->dcho);
  }
};
// Clase AVL (árbol binario de búsqueda auto-balanceado)
template <class Key>
class AVL : public ABB<Key> {
 public:
  AVL(bool traza = false) : ABB<Key>(traza) {}
  virtual ~AVL() {}
  // Insertar clave en AVL con re-balanceo
  virtual bool insertar(const Key& key) override {
    bool grew = false;
    bool inserted = false;
    this->raiz = insertarAVL(this->raiz, key, grew, inserted);
    return inserted;
  }
 protected:
  // Imprime el árbol AVL nivel a nivel mostrando factor de balance
  virtual void printLevels(std::ostream& os) const override {
    if(!this->raiz) {
      os << "Árbol vacío" << std::endl;
      os << "Nivel 0: [.]" << std::endl;
      return;
    }
    std::queue<NodoB<Key>*> q;
    q.push(this->raiz);
    int level = 0;
    while(!q.empty()) {
      int levelCount = q.size();
      os << "Nivel " << level++ << ": ";
      for(int i = 0; i < levelCount; ++i) {
        NodoB<Key>* nodo = q.front();
        q.pop();
        if(nodo) {
          NodoAVL<Key>* nodoAVL = static_cast<NodoAVL<Key>*>(nodo);
          os << "[" << nodoAVL->dato;
          if(this->traza) {
            os << "(" << nodoAVL->bal << ")";
          }
          os << "]";
          q.push(nodoAVL->izdo);
          q.push(nodoAVL->dcho);
        } else {
          os << "[.]";
          // no encolamos hijos de null
        }
        if(i < levelCount - 1) os << " ";
      }
      os << std::endl;
    }
  }
 private:
  // Inserción recursiva con balanceo AVL
  NodoB<Key>* insertarAVL(NodoB<Key>* nodo, const Key& key, bool& grew, bool& inserted) {
    if(!nodo) {
      nodo = new NodoAVL<Key>(key);
      grew = true;
      inserted = true;
      return nodo;
    }
    NodoAVL<Key>* nodoAVL = static_cast<NodoAVL<Key>*>(nodo);
    if(key < nodoAVL->dato) {
      nodoAVL->izdo = insertarAVL(nodoAVL->izdo, key, grew, inserted);
      if(grew) {
        nodoAVL->bal += 1;
        if(nodoAVL->bal == 2) {
          NodoAVL<Key>* nodoIzq = static_cast<NodoAVL<Key>*>(nodoAVL->izdo);
          if(nodoIzq->bal >= 0) {
          if(this->traza) {
            std::cout << "Desbalanceo:" << std::endl;
            this->printLevels(std::cout);
            std::cout << "Rotación II en [" << nodoAVL->dato << "(" << nodoAVL->bal << ")]:" << std::endl;
          }
          nodoAVL = rotateRight(nodoAVL);
          } else {
            if(this->traza) {
              std::cout << "Desbalanceo:" << std::endl;
              this->printLevels(std::cout);
              std::cout << "Rotación ID en [" << nodoAVL->dato << "(" << nodoAVL->bal << ")]:" << std::endl;
            }
            nodoAVL = rotateLeftRight(nodoAVL);
          }
          grew = false;
        } else if(nodoAVL->bal == 0) {
          grew = false;
        }
      }
    } else if(key > nodoAVL->dato) {
      nodoAVL->dcho = insertarAVL(nodoAVL->dcho, key, grew, inserted);
      if(grew) {
        nodoAVL->bal -= 1;
        if(nodoAVL->bal == -2) {
          NodoAVL<Key>* nodoDer = static_cast<NodoAVL<Key>*>(nodoAVL->dcho);
          if(nodoDer->bal <= 0) {
            if(this->traza) {
              std::cout << "Desbalanceo:" << std::endl;
              this->printLevels(std::cout);
              std::cout << "Rotación DD en [" << nodoAVL->dato << "(" << nodoAVL->bal << ")]:" << std::endl;
            }
          nodoAVL = rotateLeft(nodoAVL);
          } else {
            if(this->traza) {
              std::cout << "Desbalanceo:" << std::endl;
              this->printLevels(std::cout);
              std::cout << "Rotación DI en [" << nodoAVL->dato << "(" << nodoAVL->bal << ")]:" << std::endl;
            }
            nodoAVL = rotateRightLeft(nodoAVL);
          }
          grew = false;
        } else if(nodoAVL->bal == 0) {
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
  // Rotaciones AVL
  // Rotación (II)
  NodoAVL<Key>* rotateRight(NodoAVL<Key>* A) {
    NodoAVL<Key>* B = static_cast<NodoAVL<Key>*>(A->izdo);
    A->izdo = B->dcho;
    B->dcho = A;
    // Actualizar factores de balance
    if(B->bal == 1) {
      A->bal = 0;
      B->bal = 0;
    } else if(B->bal == 0) {
      A->bal = 1;
      B->bal = -1;
    }
    return B;
  }
  // Rotación (DD)
  NodoAVL<Key>* rotateLeft(NodoAVL<Key>* A) {
    NodoAVL<Key>* C = static_cast<NodoAVL<Key>*>(A->dcho);
    A->dcho = C->izdo;
    C->izdo = A;
    if(C->bal == -1) {
      A->bal = 0;
      C->bal = 0;
    } else if(C->bal == 0) {
      A->bal = -1;
      C->bal = 1;
    }
    return C;
  }
  // Rotación(ID)
  NodoAVL<Key>* rotateLeftRight(NodoAVL<Key>* A) {
    NodoAVL<Key>* B = static_cast<NodoAVL<Key>*>(A->izdo);
    NodoAVL<Key>* C = static_cast<NodoAVL<Key>*>(B->dcho);
    B->dcho = C->izdo;
    C->izdo = B;
    A->izdo = C->dcho;
    C->dcho = A;
    if(C->bal == -1) {
      B->bal = 1;
    } else {
      B->bal = 0;
    }
    if(C->bal == 1) {
      A->bal = -1;
    } else {
      A->bal = 0;
    }
    C->bal = 0;
    return C;
  }
  // Rotación (DI)
  NodoAVL<Key>* rotateRightLeft(NodoAVL<Key>* A) {
    NodoAVL<Key>* C = static_cast<NodoAVL<Key>*>(A->dcho);
    NodoAVL<Key>* B = static_cast<NodoAVL<Key>*>(C->izdo);
    C->izdo = B->dcho;
    B->dcho = C;
    A->dcho = B->izdo;
    B->izdo = A;
    if(B->bal == 1) {
      A->bal = -1;
    } else {
      A->bal = 0;
    }
    if(B->bal == -1) {
      C->bal = 1;
    } else {
      C->bal = 0;
    }
    B->bal = 0;
    return B;
  }
};


