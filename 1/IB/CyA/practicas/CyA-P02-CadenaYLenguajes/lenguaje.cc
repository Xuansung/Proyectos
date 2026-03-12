// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 2: Cadenas y lenguajes
// Autor: Xuan Sun
// Correo: alu010175411@ull.edu.es
// Fecha: 23/09/2024
// Archivo lenguaje.cc : Implementación de las funciones de lenguaje.cc.
// Contiene las funciones de la clase Lenguaje.
// Referencias:
// Enlaces de interés
// Historial de revisiones
// 19/09/2024 - Creación (primera versión) del código.
// 22/09/2024 - Desarrollo del código.
// 23/09/2024 - Adjuntar comentarios y corregir errores.

#include "lenguaje.h"

/// @brief Constructor de la clase Lenguaje
/// @param cadenas 
Lenguaje::Lenguaje(const std::vector<Cadena>& cadenas) : cadenas_(cadenas) {}

/// @brief Agrega cadenas al lenguaje.
/// @param cadena 
void Lenguaje::agregarCadena(const Cadena& cadena){
  cadenas_.push_back(cadena);
}
/// @brief Obtiene las cadenas
/// @return   cadenas_
std::vector<Cadena> Lenguaje::getCadenas() const {
  return cadenas_;
}

/// @brief Crea un lenguaje el cual contiene todos los prefijos de una cadena dada.
/// @param cadena 
/// @return Todos los prefijos de la cadena dada.
std::vector<Cadena> Lenguaje::Prefijos(const Cadena& cadena) const {
  std::vector<Cadena> prefijos;
  prefijos.push_back(CADENA_VACIA);
  std::string auxiliar_cadenas;
  for(int i = 0; i < cadena.Longitud(); i++ ){
    auxiliar_cadenas = auxiliar_cadenas + cadena.getCadena()[i] ; 
    prefijos.push_back(Cadena(auxiliar_cadenas));
  }
  return prefijos;
}

/// @brief Crea un lenguaje el cual contiene todos los sufijos de una cadena dada.
/// @param cadena 
/// @return Todos los sufijos de la cadena dada.
std::vector<Cadena> Lenguaje::Sufijos(const Cadena& cadena) const {
  std::vector<Cadena> sufijos;
  sufijos.push_back(CADENA_VACIA);
    std::string auxiliar_cadenas;
    for(int i = cadena.Longitud() - 1; i >= 0; i--){
      auxiliar_cadenas = cadena.getCadena()[i] + auxiliar_cadenas;
      sufijos.push_back(Cadena(auxiliar_cadenas));
    }
  return sufijos;
}

/// @brief Sobrecarga el operador '<<' para poder imprimir un lenguaje
/// @param os 
/// @param lenguaje 
/// @return os
std::ostream& operator<<(std::ostream& os, const Lenguaje& lenguaje){
  os << "{";
  std::vector<Cadena> lenguaje_conjunto = lenguaje.getCadenas();
  for(int i = 0; i < lenguaje_conjunto.size(); i++){
  if(!(i == lenguaje_conjunto.size() - 1))
    os << lenguaje_conjunto[i] << ", ";
  else{
    os << lenguaje_conjunto[i];
  }
  }
  os << "}";
  return os;
}
