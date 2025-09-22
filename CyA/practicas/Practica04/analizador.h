// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 4: Expresiones regulares
// Autor: Xuan Sun
// Correo: alu010175411@ull.edu.es
// Fecha: 07/10/2024
// Archivo analizador.h
// Contiene las declaraciones de la clase analizador.
// Cabecera de la clase analizador.
// Referencias:
// Enlaces de interés
// Historial de revisiones
// 05/10/2024 - Creación (primera versión) del código.
// 07/10/2024 - Revisión del código y añadir detalles y comentarios.

//HEADERS
#ifndef ANALIZADOR_H
#define ANALIZADOR_H
//Librerías usadas.
#include <vector>
#include <regex>
#include "variable.h"
#include "bucle.h"
#include "comentario.h"
#include "case.h"

//Clase Analizador junto a sus métodos y atributos.
class Analizador{
 public:
  //Métodos públicos
  Analizador(const std::string& nombre_archivo);
  void LeerArchivo(const std::string& nombre_archivo);
  void AnalizarLinea();
  void Resultados(const std::string& archivo_salida);
 private:
  //Atributos
  bool existe_main_;
  std::string nombre_archivo_;
  std::vector<std::string> lineas_codigo_;
  std::vector<Variable> variables;
  std::vector<Bucle> bucles;
  std::vector<Case> cases;
  std::vector<Comentario> comentarios_simples;
  std::vector<Comentario> comentarios_extendidos;
  //Métodos privados
  bool ExisteMain(const std::string& linea);
  void ExtraerVariable(const std::string& linea, const int numero_linea);
  void ExtraerBucle(const std::string& linea, const int numero_linea);
  void ExtraerCase(const std::string& linea, const int numero_linea);
  void ExtraerComentarioSimple(const std::string& linea, const int numero_linea);
  void ExtraerComentariosExtendidos(const std::vector<std::string>& linea,const int numero_linea);
};

#endif // ANALIZADOR_H