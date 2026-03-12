/**
 * @brief Programa principal.
 * Autor: Xuan Sun
 * Fecha: 07/04/2025
 * Programa principal para utilizar los algoritmos de ordenación.
 */
// Librerías
#include <iostream>
#include <fstream>
#include "nif.h"
#include "sequence.h"
#include "sortmethod.h"
#include "sortfunction.h"

int main(int argc, char* argv[]) {
  // Variables para almacenar los parámetros de línea de comandos
  size_t tam = 0;            // Tamaño de la secuencia (-size)
  int metodo = -1;           // Código del método de ordenación (-ord)
  std::string initMode;      // Modo de inicialización (-init): "manual", "random" o "file"
  std::string filename;      // Nombre de fichero (si initMode == "file")
  bool trace = false;        // Flag de traza (-trace: 'y' o 'n')

  // Procesar los argumentos de la línea de comandos
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "-size" && i + 1 < argc) {
      // Leer el siguiente argumento como el tamaño de la secuencia
      tam = std::stoi(argv[++i]);
    } else if (arg == "-ord" && i + 1 < argc) {
      // Leer el código del método de ordenación
      metodo = std::stoi(argv[++i]);
    } else if (arg == "-init" && i + 1 < argc) {
      // Leer el modo de inicialización y, si es "file", obtener el nombre de fichero
      initMode = argv[++i];
      if (initMode == "file" && i + 1 < argc) {
        filename = argv[++i];
      }
    } else if (arg == "-trace" && i + 1 < argc) {
      // Leer si se desea mostrar la traza ('y' para sí, 'n' para no)
      std::string t = argv[++i];
      trace = (t == "y" || t == "Y");
    } else {
      // Argumento no reconocido (opcionalmente se podría mostrar un mensaje de error)
      // En este caso, ignoramos argumentos desconocidos.
    }
  }

  // Validar que los parámetros esenciales han sido proporcionados
  if (tam == 0 || metodo < 0 || initMode.empty()) {
    std::cerr << "Uso: " << argv[0]  << " -size <tam> -ord <metodo> -init <manual|random|file [fichero]> -trace <y|n>\n";
    return 1;
  }

  // Crear el objeto que implementa el método de ordenación seleccionado
  SortMethod<NIF>* sorter = nullptr;
  double alfa = 0.0;
  switch (metodo) {
    case 1:  // Inserción
      sorter = new InsertionSort<NIF>(tam, trace);
      break;
    case 2:  // Sacudida (ShakeSort)
      sorter = new ShakeSort<NIF>(tam, trace);
      break;
    case 3:  // QuickSort
      sorter = new QuickSort<NIF>(tam, trace);
      break;
    case 4:  // HeapSort
      sorter = new HeapSort<NIF>(tam, trace);
      break;
    case 5:  // ShellSort (por incrementos decrecientes)
      // Solicitar al usuario el valor de alfa si no se pasó como argumento
      std::cout << "Introduzca el valor de alfa (0 < alfa < 1): ";
      std::cin >> alfa;
      if (alfa <= 0.0 || alfa >= 1.0) {
        std::cout << "Valor de alfa no válido, se usará 0.4545 por defecto.\n";
        alfa = 0.5;
      }
      sorter = new ShellSort<NIF>(tam, trace, alfa);
      break;
    default:
      std::cerr << "Método de ordenación no reconocido.\n";
      return 1;
  }

  // Rellenar la secuencia con los datos según el modo de inicialización indicado
  if (initMode == "manual") {
    // Introducción manual de los datos por teclado
    std::cout << "Introduzca los " << tam << " elementos de la secuencia:" << std::endl;
    for (size_t i = 0; i < tam; ++i) {
      std::cin >> sorter->getSequence()[i];  // Leer cada elemento (NIF) desde std::cin
    }
  } else if (initMode == "random") {
    // Inicialización aleatoria de la secuencia
    std::srand(std::time(nullptr));  // Semilla para números aleatorios basada en el tiempo actual
    for (size_t i = 0; i < tam; ++i) {
      sorter->getSequence()[i] = NIF();  // Cada NIF creado con el constructor por defecto es aleatorio
    }
  } else if (initMode == "file") {
    // Inicialización leyendo los valores desde un fichero
    std::ifstream filein(filename);
    if (!filein) {
      std::cerr << "Error al abrir el fichero: " << filename << std::endl;
      delete sorter;
      return 1;
    }
    for (size_t i = 0; i < tam; ++i) {
      if (!(filein >> sorter->getSequence()[i])) {
        // Si el fichero tiene menos datos de los esperados o ocurre un error de lectura, salir
        break;
      }
    }
    filein.close();
  }

  // Ejecutar el algoritmo de ordenación seleccionado
  sorter->Sort();

  // Mostrar el resultado de la secuencia ordenada
  std::cout << "Secuencia ordenada: ";
  for (size_t i = 0; i < tam - 1; ++i) {
    std::cout << sorter->getSequence()[i] << " ";
  }
  std::cout << std::endl;

  // Liberar la memoria asignada para el objeto de ordenación
  delete sorter;
  return 0;
}
