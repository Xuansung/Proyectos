/*
 * Archivo: main.cc
 * 
 * Descripción:
 * Programa principal para la gestión de árboles de búsqueda (ABB y AVL) con claves de tipo NIF.
 * 
 * Funcionalidades:
 *  - Creación de árboles binarios de búsqueda (ABB) o balanceados (AVL).
 *  - Inicialización del árbol manualmente, aleatoriamente o desde un fichero.
 *  - Inserción de nuevas claves en el árbol.
 *  - Búsqueda de claves en el árbol.
 *  - Visualización del árbol en orden (recorrido inorden).
 *  - Opción de seguimiento detallado (traza) de las operaciones de inserción y búsqueda.
 * 
 * Parámetros de ejecución:
 *  -ab [abb|avl]    : Tipo de árbol a utilizar (por defecto ABB).
 *  -trace [y|n]     : Activar o desactivar el modo de traza (por defecto desactivado).
 *  -init [manual|random|file] [n] [nombre_fichero] : Método de inicialización del árbol.
 *        * manual: inserción manual de elementos.
 *        * random: generación aleatoria de 'n' elementos (por defecto 10).
 *        * file: lectura de elementos desde fichero (necesario nombre de fichero).
 * 
 * Uso:
 *  ./programa -ab avl -trace y -init random 20
 *  ./programa -init file 15 datos.txt
 * 
 * Autor: Xuan Sun
 * Fecha: 27/04/2025
 */

// Librerías
#include <fstream>
#include "nodo.h"
#include "arbol.h"
#include "nif.h"

int main(int argc, char* argv[]) {
  // Parámetros por defecto
  bool useAVL = false;
  bool useABE = false;
  bool trace = false;
  bool initManual = true;
  bool initRandom = false;
  bool initFile = false;
  int randomCount = 0;
  std::string fileName = "";

  // Leer argumentos de línea de comandos
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "-ab" && i + 1 < argc) {
      std::string type = argv[++i];
      if (type == "avl") useAVL = true;
      else if (type == "abe") useABE = true;
      else if (type == "abb") useAVL = false;
    } else if (arg == "-trace" && i + 1 < argc) {
      std::string t = argv[++i];
      trace = (t == "y" || t == "Y" || t == "1");
    } else if (arg == "-init" && i + 1 < argc) {
      std::string mode = argv[++i];
      if (mode == "manual") {
        initManual = true;
      } else if (mode == "random") {
        initManual = false;
        initRandom = true;
        initFile = false;
        // número de elementos aleatorios (opcional)
        if (i + 1 < argc && std::isdigit(argv[i + 1][0])) {
          randomCount = std::atoi(argv[++i]);
        } else {
          randomCount = 10;  // valor por defecto
        }
      } else if (mode == "file") {
        initManual = false;
        initRandom = false;
        initFile = true;
        // leer opcionalmente número y nombre de fichero
        if (i + 1 < argc && std::isdigit(argv[i + 1][0])) {
          randomCount = std::atoi(argv[++i]);
        }
        if (i + 1 < argc) {
          fileName = argv[++i];
        } else {
          std::cerr << "Error: nombre de fichero no proporcionado\n";
          return 1;
        }
      }
    }
  }

  // Crear el árbol según -ab
  AB<NIF>* arbol;
  if (useAVL) arbol = new AVL<NIF>(trace);
  else if (useABE) arbol = new ABE<NIF>(trace);
  else arbol = new ABB<NIF>(trace);

  // Inicializar el árbol según -init
  if (initRandom) {
    std::srand(std::time(NULL));
    int n = (randomCount > 0 ? randomCount : 10);
    for (int k = 0; k < n; ++k) {
      NIF val;
      arbol->insertar(val);
    }
  } else if (initFile) {
    std::ifstream file(fileName);
    if (!file) {
      std::cerr << "Error al abrir el fichero: " << fileName << std::endl;
      delete arbol;
      return 1;
    }
    NIF val;
    int count = 0;
    while (file >> val) {
      arbol->insertar(val);
      if (randomCount > 0 && ++count >= randomCount) break;
    }
    file.close();
  }
  // Mostrar estado inicial del árbol
  std::cout << *arbol;

  // Bucle de menú interactivo
  int opcion;
  NIF clave;
  do {
    std::cout << std::endl;
    std::cout << "[0] Salir" << std::endl;
    std::cout << "[1] Insertar clave" << std::endl;
    std::cout << "[2] Buscar clave" << std::endl;
    std::cout << "[3] Mostrar árbol inorden" << std::endl;
    std::cout << "Opción: ";
    if (!(std::cin >> opcion)) break;  // salir si entrada inválida
    switch (opcion) {
      case 0:
        // Salir
        break;
      case 1:
        std::cout << "Valor a insertar: ";
        std::cin >> clave;
        std::cout << "Insertar: " << clave << std::endl;
        if (!arbol->insertar(clave)) {
          std::cout << "La clave ya existe en el árbol." << std::endl;
        }
        std::cout << *arbol;
        break;
      case 2:
        std::cout << "Valor a buscar: ";
        std::cin >> clave;
        if (arbol->buscar(clave)) {
          std::cout << "Clave " << clave << " encontrada en el árbol." << std::endl;
        } else {
          std::cout << "Clave " << clave << " no encontrada en el árbol." << std::endl;
        }
        break;
      case 3:
        std::cout << "Recorrido inorden: ";
        arbol->inorden(std::cout);
        break;
      default:
        std::cout << "Opción no válida." << std::endl;
    }
  } while (opcion != 0);

  // Liberar memoria
  delete arbol;
  return 0;
}
