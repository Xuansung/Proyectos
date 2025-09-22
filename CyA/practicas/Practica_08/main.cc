// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 8: Gramáticas en Forma Normal de Chomsky.
// Autor: Xuan Sun
// Correo: alu010175411@ull.edu.es
// Fecha: 05/11/2024
// Archivo main.cc
// Programa principal main.cc
// Referencias:
// Enlaces de interés
// Historial de revisiones
// 01/11/2024 - Creación (primera versión) del código.
// 05/11/2024 - Última modificación.

#include "gramatica.h"

void Ayuda() {
  std::cout << "Uso ./Grammar2CNF input.gra output.gra\n";
  std::cout << "Convierte una gramática independiente del contexto a forma normal de Chomsky.\n";
  std::cout << "Argumentos:\n";
  std::cout << "input.gra Archivo de entrada con la gramática original. \n";
  std::cout << "output.gra Archivo de salida para la gramática en FNC. \n";
  std::cout << "Opciones:\n";
  std::cout << " --help Muestra la ayuda.\n"; 
}

int main(int argc, char* argv[]) {
  if ((argv[1]) == "--help") {
    Ayuda();
    return 0;
  }

  if (argc !=3) {
    std::cerr << "ERROR: NÚMERO INCORRECTO DE ARGUMENTOS.\n";
    Ayuda();
    return 1;
  }

  std::string archivo_entrada = argv[1];
  std::string archivo_salida = argv[2];

  Gramatica gramatica;

  if (!gramatica.CargarArchivo(archivo_entrada)) {
    std::cerr << "Erro al cargar la gramática desde el archivo " << archivo_entrada << ".\n";
    return 1;
  }

  gramatica.ConvertirCNF();

  if(!gramatica.GuardarArchivo(archivo_salida)) {
    std::cerr << "Error al guardar la gramática en el archivo " << archivo_salida << ".\n";
    return 1;
  }
  return 0;
}