// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 2: Cadenas y lenguajes
// Autor: Xuan Sun
// Correo: alu010175411@ull.edu.es
// Fecha: 23/09/2024
// Archivo main.cc
// Contiene el int main, es el programa cliente .
// Referencias:
// Enlaces de interés
// Historial de revisiones
// 19/09/2024 - Creación (primera versión) del código.
// 22/09/2024 - Desarrollo del código.
// 23/09/2024 - Adjuntar comentarios y corregir errores.

#include "lenguaje.h"
#include <fstream>
#include <iostream>

/// @brief Apartado por el cual ayuda al usuario a usar el programa.
void Mostrar_help(){
  std::cout << "Modo de empleo del programa:\n";
  std::cout << "./programa_cliente [filein.txt] [fileout.txt] [opcode] [Numero de la potencia](Exclusivo para N)\n";
  std::cout << "[filein.txt] es el fichero de entrada por el cual se leen los valores con el que se va a trabajar. \n";
  std::cout << "[fileout.txt] es el fichero por el cual se va a imprimir los resultados del programa.\n";
  std::cout << "[opcode] es la operación que quieres utilizar. Constan de las siguientes opciones:\n";
  std::cout << "A: Alfabeto, escribe en el fichero de salida el alfabeto asociado a una de las cadenas de entrada.\n";
  std::cout << "L: Longitud, escribe en el fichero de salida la longitud de cada cadena de entrada.\n";
  std::cout << "I: Inversa, escribe en el fichero de salida la inversa de cada cadena de entrada.\n";
  std::cout << "P: Prefijos, escribe en el fichero de salida el conjunto de cadenas que son prefijos de la cadena de entrada correspondiente.\n"; 
  std::cout << "S: Sufijos, escribe en el fichero de salida el conjunto de cadenas que son sufijos de cada cadena de entrada correspondiente.\n";
  std::cout << "N: Potencia, escribe la cadena elevado a n\n";
}

int main(int argc, char* argv[]){
  
  std::string ayuda = "--help";
  if(std::string(argv[1]) == ayuda){
    Mostrar_help();
    return 1;
  }
  //Verifica si la entrada es la correcta.
  if(argc != 5 && argc != 4){
    std::cout << "Modo de empleo: ./programa_cliente [filein.txt] [fileout.txt] [opcode] [Numero de la potencia](Exclusivo para N)\n";
    std::cout << "Pruebe ./programa_cliente '--help' para más información. \n";
    return 1;
  }
  std::ifstream fichero_entrada(argv[1]); // fichero de entrada
  std::ofstream fichero_salida(argv[2]); // fichero de salida
  char opcode = argv[3][0]; // OPCODE

  //Verifica si ambos ficheros se han abierto correctamente
  if(!fichero_entrada.is_open() || !fichero_salida.is_open()){
    std::cerr << " Error al abrir los ficheros\n";
    return 1;
  }
  //Lectura de la entrada.
  std::string cadena_entrada, alfabeto_entrada;
  while(fichero_entrada >> cadena_entrada >> alfabeto_entrada){
    Alfabeto alfabeto(alfabeto_entrada);
    Cadena cadena(cadena_entrada);
    Lenguaje lenguaje({cadena});
    // Opciones para el OPCODE.
    if(argc > 4){
      const int numero = std::stoi(argv[4]); // Numero de la potencia
      switch(opcode){
        case 'N': 
          fichero_salida << cadena.Potencia(numero) << std::endl;
        break;
      }
    }else{
      switch(opcode){
        case 'A':
          fichero_salida << alfabeto << std::endl;
          break;
        case 'L':
          fichero_salida << cadena.Longitud() << std::endl;
          break;
        case 'I':
          fichero_salida << cadena.Inversa() << std::endl;
          break;
        case 'P':
          fichero_salida << lenguaje.Prefijos(cadena) << std::endl;
          break;
        case 'S':
          fichero_salida << lenguaje.Sufijos(cadena) << std::endl;
          break;
        default:
          std::cout << "Error en la introducción de Opcode, escribe '--help' para más información\n";
          break;
      }
    }
  }
  //Cerrar los ficheros.
  fichero_entrada.close();
  fichero_salida.close();
  return 0;

}