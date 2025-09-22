// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 5: Construcción de un Autómata Finito
// Autor: Xuan Sun
// Correo: alu010175411@ull.edu.es
// Fecha: 22/10/2024
// Archivo main.cc
// Programa principal main.cc
// Referencias:
// Enlaces de interés
// Historial de revisiones
// 20/10/2024 - Creación (primera versión) del código.
// 22/10/2024 - Última modificación.

#include <iostream>
#include "automata_finito.h"

/// @brief Muestra la ayuda
/// @param argv 
void MostrarAyuda(char* argv[]){
  std::cout << "Este programa simula el comportamiento de un autómata finito (FA). \n";
  std::cout << "Modo de empleo: " << argv[0] << " input.fa input.txt \n";
  std::cout << "El fichero input.fa debe contener la especificación del autómata finito en el siguiente formato: \n";
  std::cout <<  "Línea 1: Símbolos del alfabeto separados por espacios. \n";
  std::cout << "Línea 2: Número total de estados del autómata.\n";
  std::cout << "Línea 3: Estado de arranque del autómata.\n";
  std::cout << "A continuación, una línea por cada estado, con el formato:\n";
  std::cout << "EstadoID EsAceptación(1 o 0) NumTransiciones [Transiciones...]\n";
  std::cout << "Cada transición está definida por: SímboloDestino EstadoDestino\n";
  std::cout << "Utilice '&' para representar la cadena vacía en las transiciones.\n";
}

int main(int argc, char* argv[]){
  if (argc == 1) {
    std::cout << "Modo de empleo: " << argv[0] << " input.fa input.txt \n";
    std::cout << "Pruebe " << argv[0] << "--help para más información. \n";
    return 1; 
  } else if (argc == 2 && std::string(argv[1]) == "--help") {
    MostrarAyuda(argv);
    return 1;
  } else if (argc != 3) {
    std::cerr << "Número incorrecto de argumentos." << std::endl;
    std::cout << "Modo de empleo: " << argv[0] << " input.fa input.txt \n";
    std::cout << "Pruebe " << argv[0] << "--help para más información. \n"; 
    return 1;
  }

  std::string archivo_fa = argv[1];
  std::string archivo_txt = argv[2];

  AutomataFinito automata_finito;
  if(!automata_finito.LeerArchivo(archivo_fa)) {
    return 1;
  }

  std::ifstream archivo_cadenas(archivo_txt);
  if (!archivo_cadenas.is_open()) {
    std::cerr << "No se pudo el archivo de cadenas: " << archivo_txt << std::endl;
    return 1;
  }
  
  std::string linea;
  while (getline(archivo_cadenas, linea)) {
    if(linea.empty()) continue;

    std::istringstream iss(linea);
    std::string  cadena;
    if (!(iss >> cadena)) {
      std::cerr << "Error leyendo la línea: " << linea << std::endl;
      continue;
    }
    if( cadena == "&") {
        cadena = "";
    }

    bool aceptada = automata_finito.SimularCadenas(cadena);

    std::cout << (cadena.empty() ? "&" : cadena) << "---" << (aceptada ? "Accepted" : "Rejected") << std::endl;
  }
  archivo_cadenas.close();
  return 0;
}