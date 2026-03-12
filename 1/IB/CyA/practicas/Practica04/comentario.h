// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 4: Expresiones regulares
// Autor: Xuan Sun
// Correo: alu010175411@ull.edu.es
// Fecha: 07/10/2024
// Archivo comentario.h
// Contiene las declaraciones de la clase comentario.
// Cabecera de la clase comentario.
// Referencias:
// Enlaces de interés
// Historial de revisiones
// 05/10/2024 - Creación (primera versión) del código.
// 07/10/2024 - Revisión del código y añadir detalles y comentarios.

//HEADERS
#ifndef COMENTARIO_H
#define COMENTARIO_H
//Librerías usadas
#include <string>

//Clase Comentario
class Comentario{
 public:
  //Métodos
  Comentario(const std::string& tipo_comentario, const std::string& contenido_comentario,const int linea_inicio, const int linea_final);
  std::string getContenido_comentario() const;
  int getLineaInicio() const;
  int getLineaFinal() const;
  std::string getTipo_comentario() const;
 private:
  //Atributos
  const std::string tipo_comentario_;
  const std::string contenido_comentario_;
  int linea_inicio_;
  int linea_final_;
};

#endif // COMENTARIO_H