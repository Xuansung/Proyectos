// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 4: Expresiones regulares
// Autor: Xuan Sun
// Correo: alu010175411@ull.edu.es
// Fecha: 07/10/2024
// Archivo main.cc
// Programa principal.
// Referencias:
// Enlaces de interés
// Historial de revisiones
// 05/10/2024 - Creación (primera versión) del código.
// 07/10/2024 - Revisión del código y añadir detalles y comentarios.


#include "analizador.h"
#include <iostream>

int main(int argc, char* argv[]){
  if(argc != 3){
    std::cerr << "Error en los argumentos dados. Debe de ser " << argv[0] << " <archivo_entrada> <archivo_salida>\n"
    << "Siendo <archivo_entrada> un programa de C++ y <archivo_salida> un archivo .txt.\n";
    return 1;
  }
  std::string archivo_entrada = argv[1];
  std::string archivo_salida = argv[2];
  Analizador analizador(archivo_entrada);
  //Abre y lee el archivo de entrada.
  analizador.LeerArchivo(archivo_entrada);
  //Analiza las líneas del archivo.
  analizador.AnalizarLinea();
  //Imprime los resultados.
  analizador.Resultados(archivo_salida);

  return 0;
}