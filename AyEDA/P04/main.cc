/*
* Autor: Xuan Sun
* Fecha: 22/03/2025
* Resumen: Programa principal para usar el HashTable
*/
#include <iostream>
#include <cstring>
#include <string>
#include "nif.h"
#include "hash.h"
#include "dispersion.h"
#include "exploration.h"

int main(int argc, char* argv[]) {
  // Parámetros con valores por defecto
  unsigned tableSize = 0;
  unsigned blockSize = 0;
  std::string dispersionCode;
  std::string hashType;
  std::string explorationCode;
  // Parsear los argumentos de línea de comandos
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "-ts" && i + 1 < argc) {
      tableSize = std::stoi(argv[++i]);
      } else if (arg == "-fd" && i + 1 < argc) {
        dispersionCode = argv[++i];
      } else if (arg == "-hash" && i + 1 < argc) {
        hashType = argv[++i];
      } else if (arg == "-bs" && i + 1 < argc) {
        blockSize = std::stoi(argv[++i]);
      } else if (arg == "-fe" && i + 1 < argc) {
        explorationCode = argv[++i];
    }
  }

  // Validar parámetros
  if (tableSize == 0 || dispersionCode.empty() || hashType.empty() || (hashType == "close" && (blockSize == 0 || explorationCode.empty()))) {
    std::cerr << "Uso: " << argv[0] << " -ts <tam_tabla> -fd <mod|sum|rand> -hash <open|close> "
    << "[-bs <tam_bloque> -fe <lineal|cuadratica|doble|redisp>]\n";
    return 1;
  }

  // Crear función de dispersión
  DispersionFunction<NIF>* fd = nullptr;
  if (dispersionCode == "mod") {
    fd = new ModuleDispersion<NIF>(tableSize);
  } else if (dispersionCode == "sum") {
    fd = new SumDispersion<NIF>(tableSize);
  } else if (dispersionCode == "rand") {
    fd = new PseudoRandomDispersion<NIF>(tableSize);
  } else {
    std::cerr << "Error: función de dispersión desconocida.\n";
    return 1;
  }

  // Crear función de exploración (si procede)
  ExplorationFunction<NIF>* fe = nullptr;
  DispersionFunction<NIF>* secondFd = nullptr;
  if (hashType == "close") {
    if (explorationCode == "lineal") {
      fe = new LinealExploration<NIF>();
    } else if (explorationCode == "cuadratica") {
      fe = new CuadraticExplorati// A la hora de compilar añadirle un -D para saber si tener la versión sin traza o con traza.on<NIF>();
    } else if (explorationCode == "doble") {
    // Se utiliza una segunda función de dispersión auxiliar
    if (dispersionCode != "sum") 
      secondFd = new SumDispersion<NIF>(tableSize);
    else                         
      secondFd = new ModuleDispersion<NIF>(tableSize);
    fe = new DoubleDispersion<NIF>(*secondFd);
    } else if (explorationCode == "redisp") {
      fe = new ReDispersion<NIF>();
    } else {
      std::cerr << "Error: función de exploración desconocida.\n";
        delete fd;
        return 1;
      }
  }

  // Construir la tabla hash y mostrar menú
  if (hashType == "open") {
    HashTable<NIF, DynamicSequence<NIF>> hashTable(tableSize, *fd);
    std::cout << "Tabla hash (abierta) creada. Tam: " << tableSize << "\n";
    int opcion;
    do {
      std::cout << "\nMenu:\n1. Insertar\n2. Buscar\n0. Salir\nOpcion: ";
      std::cin >> opcion;
      if (opcion == 1) {
        std::cout << "NIF (8 dígitos): ";
        long nif; 
        std::cin >> nif;
        NIF clave(nif);
        bool ok = hashTable.insert(clave);
        std::cout << (ok ? "Insertado\n" : "No insertado\n");
      } else if (opcion == 2) {
        std::cout << "NIF a buscar: ";
        long nif; 
        std::cin >> nif;
        NIF clave(nif);
        bool found = hashTable.search(clave);
        std::cout << (found ? "Encontrado\n" : "NO encontrado\n");
      }
    } while (opcion != 0);
  } else {
    HashTable<NIF, StaticSequence<NIF>> hashTable(tableSize, blockSize, *fd, *fe);
    std::cout << "Tabla hash (cerrada) creada. Tam: " << tableSize << ", bloque: " << blockSize << "\n";
    int opcion;
    do {
      std::cout << "\nMenu:\n1. Insertar\n2. Buscar\n0. Salir\nOpcion: ";
      std::cin >> opcion;
      if (opcion == 1) {
        std::cout << "NIF (8 dígitos): ";
        long nif; 
        std::cin >> nif;
        NIF clave(nif);
        bool ok = hashTable.insert(clave);
        std::cout << (ok ? "Insertado\n" : "No insertado\n");
      } else if (opcion == 2) {
        std::cout << "NIF a buscar: ";
        long nif; 
        std::cin >> nif;
        NIF clave(nif);
        bool found = hashTable.search(clave);
        std::cout << (found ? "Encontrado\n" : "NO encontrado\n");
      }
    } while (opcion != 0);
  }

  // Liberar recursos
  delete fd;
  if (fe) delete fe;
  if (secondFd) delete secondFd;
  return 0;
}
