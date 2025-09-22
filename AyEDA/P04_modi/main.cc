/*
* Autor: Xuan Sun
* Fecha: 22/03/2025
* Resumen: Programa principal para usar el HashTable
*/
#include <iostream>
#include <cstring>
#include <string>
#include "matricula.h"
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
  DispersionFunction<Matricula>* fd = nullptr;
  if (dispersionCode == "mod") {
    fd = new ModuleDispersion<Matricula>(tableSize);
  } else if (dispersionCode == "sum") {
    fd = new SumDispersion<Matricula>(tableSize);
  } else if (dispersionCode == "rand") {
    fd = new PseudoRandomDispersion<Matricula>(tableSize);
  } else {
    std::cerr << "Error: función de dispersión desconocida.\n";
    return 1;
  }

  // Crear función de exploración (si procede)
  ExplorationFunction<Matricula>* fe = nullptr;
  DispersionFunction<Matricula>* secondFd = nullptr;
  if (hashType == "close") {
    if (explorationCode == "lineal") {
      fe = new LinealExploration<Matricula>();
    } else if (explorationCode == "cuadratica") {
      fe = new CuadraticExploration<Matricula>();
    } else if (explorationCode == "doble") {
    // Se utiliza una segunda función de dispersión auxiliar
    if (dispersionCode != "sum") 
      secondFd = new SumDispersion<Matricula>(tableSize);
    else                         
      secondFd = new ModuleDispersion<Matricula>(tableSize);
    fe = new DoubleDispersion<Matricula>(*secondFd);
    } else if (explorationCode == "redisp") {
      fe = new ReDispersion<Matricula>();
    } else {
      std::cerr << "Error: función de exploración desconocida.\n";
        delete fd;
        return 1;
      }
  }

  // Construir la tabla hash y mostrar menú
  if (hashType == "open") {
    HashTable<Matricula, DynamicSequence<Matricula>> hashTable(tableSize, *fd);
    std::cout << "Tabla hash (abierta) creada. Tam: " << tableSize << "\n";
    int opcion;
    do {
      std::cout << "\nMenu:\n1. Insertar\n2. Buscar\n0. Salir\nOpcion: ";
      std::cin >> opcion;
      if (opcion == 1) {
        std::cout << "Matricula(4 valores numéricos y 3 letras): ";
        std::string matricula;
        std::cin >> matricula;
        Matricula clave(matricula);
        std::cout << "Marca: ";
        std::string marca; 
        std::cin >> marca;
        clave.setMarca(marca);
        bool ok = hashTable.insert(clave);
        std::cout << (ok ? "Insertado\n" : "No insertado\n");
      } else if (opcion == 2) {
        std::cout << "Matricula a buscar: ";
        std::string matricula;
        std::cin >> matricula;
        Matricula clave(matricula);
        bool found = hashTable.search(clave);
        std::cout << (found ? "Encontrado\n" : "NO encontrado\n");
      }
    } while (opcion != 0);
  } else {
    HashTable<Matricula, StaticSequence<Matricula>> hashTable(tableSize, blockSize, *fd, *fe);
    std::cout << "Tabla hash (cerrada) creada. Tam: " << tableSize << ", bloque: " << blockSize << "\n";
    int opcion;
    do {
      std::cout << "\nMenu:\n1. Insertar\n2. Buscar\n0. Salir\nOpcion: ";
      std::cin >> opcion;
      if (opcion == 1) {
        std::cout << "Matricula(4 valores numéricos y 3 letras): ";
        std::string matricula;
        std::cin >> matricula;
        Matricula clave(matricula);
        std::cout << "Marca: ";
        std::string marca; 
        std::cin >> marca;
        clave.setMarca(marca);
        bool ok = hashTable.insert(clave);
        std::cout << (ok ? "Insertado\n" : "No insertado\n");
      } else if (opcion == 2) {
        std::cout << "Matricula a buscar: ";
        std::string matricula; 
        std::cin >> matricula;
        Matricula clave(matricula);
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
