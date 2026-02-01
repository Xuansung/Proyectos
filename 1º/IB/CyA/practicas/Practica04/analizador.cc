// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 4: Expresiones regulares
// Autor: Xuan Sun
// Correo: alu010175411@ull.edu.es
// Fecha: 07/10/2024
// Archivo analizador.cc
// Contiene las funciones de la clase analizador.cc.
// Funciones de la clase Analizador.
// Referencias:
// Enlaces de interés
// Historial de revisiones
// 05/10/2024 - Creación (primera versión) del código.
// 07/10/2024 - Revisión del código y añadir detalles y comentarios.

//Librerías usadas.
#include "analizador.h"
#include <iostream>
#include <fstream>

//Constructor de la clase.
Analizador::Analizador(const std::string& nombre_archivo) : nombre_archivo_(nombre_archivo) ,existe_main_(false) {}

/// @brief Lee el archvivo de entrada, analizando cada línea del fichero para guardarlo en una variable.
/// @param nombre_archivo 
void Analizador::LeerArchivo(const std::string& nombre_archivo){
  nombre_archivo_ = nombre_archivo;
  std::ifstream archivo_entrada(nombre_archivo);
  //Comprueba si el archivo de entrada se puede abrir o no.
  if(!archivo_entrada.is_open()){
    std::cerr << " No se pudo abrir el archivo \n";
    exit(EXIT_FAILURE);
  }
  std::string linea;
  //Inserta cada línea en la string.
  while(std::getline(archivo_entrada,linea)){
    lineas_codigo_.push_back(linea);
  }
  archivo_entrada.close();
}

/// @brief Analiza cada línea del fichero y llama a los demás métodos para que trabajen con ello.
void Analizador::AnalizarLinea(){
  for(int linea_numero = 0; linea_numero < lineas_codigo_.size(); linea_numero++){
    ExtraerVariable(lineas_codigo_[linea_numero], linea_numero + 1 );
    ExtraerBucle(lineas_codigo_[linea_numero], linea_numero + 1 );
    ExtraerCase(lineas_codigo_[linea_numero], linea_numero + 1 );
    ExtraerComentarioSimple(lineas_codigo_[linea_numero], linea_numero + 1);
    ExtraerComentariosExtendidos(lineas_codigo_,linea_numero + 1);
    if(ExisteMain(lineas_codigo_[linea_numero])){
      existe_main_ = true;
    }
  }
}

/// @brief Extrae las variables tipo int o double de cada línea del archivo y las guarda en "variables".
/// @param linea 
/// @param numero_linea 
void Analizador::ExtraerVariable(const std::string& linea, const int numero_linea){
  // Variables int o double declaradas por un "=".
  std::regex regex_variable_igual(R"(^\s*(int|double)\s+(\w+)\s*(=\s*(\d+\.?\d*?))?\s*;)");
  // Variables int o double declaradas por "{}".
  std::regex regex_variable_llaves(R"(^\s*(int|double)\s+(\w+)\s*(\s*\{(\d+\.?\d*?)\})?\s*;)");
  std::smatch matches;
  if(std::regex_search(linea, matches, regex_variable_igual)){
    std::string tipo_variable = matches[1];
    std::string nombre = matches[2];
    std::string valor = matches[4];
    variables.emplace_back(tipo_variable, nombre, valor, numero_linea);
  }else if(std::regex_search(linea, matches, regex_variable_llaves)){
    std::string tipo_variable = matches[1];
    std::string nombre = matches[2];
    std::string valor = matches[4];
    variables.emplace_back(tipo_variable, nombre, valor, numero_linea);
  }
}

/// @brief Extrae los bucles de cada línea del archivo y las guarda en "bucles".
/// @param linea 
/// @param numero_linea 
void Analizador::ExtraerBucle(const std::string& linea, const int numero_linea){
  std::regex regex_bucle(R"(\s*(for|while)\s*\(.+\)\s*\{)");
  std::smatch match;
  if(std::regex_search(linea, match, regex_bucle)){
    std::string tipo_bucle = match[1];
    bucles.emplace_back(tipo_bucle, numero_linea);
  }
}

/// @brief Extrae los cases de una sola línea del archivo y las guarda en "cases".
/// @param linea 
/// @param numero_linea 
void Analizador::ExtraerCase(const std::string& linea, const int numero_linea){
  std::regex regex_case(R"(\s*case\s(\'?.\'?)\s?:$)");
  std::smatch match;
  if(std::regex_search(linea,match,regex_case)){
    std::string valor = match[1];
    cases.emplace_back(valor, numero_linea);
  }
}

/// @brief Extrae los comentarios de una sola línea del archivo y las guarda en "comentarios_simples".
/// @param linea 
/// @param numero_linea 
void Analizador::ExtraerComentarioSimple(const std::string& linea, const int numero_linea){
  std::regex regex_comentario_simple(R"(^(?!\s*\*).*\/\/(.*))");
  std::smatch match;
  if(std::regex_search(linea,match, regex_comentario_simple)){
    std::string contenido_comentario = match[1];
    comentarios_simples.emplace_back("//",contenido_comentario, numero_linea, numero_linea);
  }
}

/// @brief Extrae los comentarios extendidos como la descripción y las guarda en "comentarios_extendidos".
/// @param linea 
/// @param numero_linea 
void Analizador::ExtraerComentariosExtendidos(const std::vector<std::string>& linea,const int numero_linea){
  std::regex regex_comentario_extendido_inicio(R"(\/\*.*)");
  std::regex regex_comentario_extendido_final(R"(.*\*\/)");
  bool dentro_comentario = false;
  std::string comentario;
  int linea_inicio = 0;
  int linea_final = 0;
  //Busca por todas las líneas del código.
  for (int i = 0; i < linea.size(); ++i) {
    //Si la línea no está dentro del comentario seleccionado y encuentra "/* empieza a buscar"
    if (!dentro_comentario && std::regex_search(linea[i], regex_comentario_extendido_inicio)) {
      dentro_comentario = true;
      linea_inicio = i + 1;
      comentario = linea[i];
      //Si la línea está dentro del comentario, lo empieza a incluir.
    }else if (dentro_comentario) {
      comentario += "\n" + linea[i];
      //Si encuentra "*/" deja de buscar.
      if (std::regex_search(linea[i], regex_comentario_extendido_final)) {
        dentro_comentario = false;
        linea_final = i + 1;
      }
    }
  }
  comentarios_extendidos.emplace_back("/**/", comentario, linea_inicio, linea_final);
}

/// @brief Analiza si existe int main en el archivo. 
/// @param linea 
/// @return 1 si existe main , 0 si no existe.
bool Analizador::ExisteMain(const std::string& linea){
  std::regex regex_main(R"(\s*(int main\s*\(.*\))\s*\{)");
  std::smatch match;
  if(std::regex_search(linea, match, regex_main)){
    return true;
  } else{
    return false;
  }
}

/// @brief Imprime los resultados del análisis en un archivo de salida. 
/// @param archivo_salida 
void Analizador::Resultados(const std::string& archivo_salida){
  std::ofstream archivo_resultante(archivo_salida);
  //Comprueba si el archivo de salida se ha creado correctamente o no.
  if(!archivo_resultante.is_open()){
    std::cerr << "Error al intentar crear el archivo de salida \n";
    exit(EXIT_FAILURE);
  }  

  //Inserta el nombre del archivo de entrada.
  archivo_resultante << "PROGRAM: " << nombre_archivo_ << std::endl;

  //Inserta la descripción del código.
  archivo_resultante << "\nDESCRIPTION:\n";
  archivo_resultante << comentarios_extendidos[0].getContenido_comentario() << std::endl;

  //Inserta las variables,su tipo, la línea en la que aparece y su valor.
  archivo_resultante << "\nVARIABLES :\n";
  for( int i = 0 ; i < variables.size(); i++){
    archivo_resultante << "[Line " << variables[i].getLinea() << " ] ";
    if(variables[i].getTipo_variable() == "int")
      archivo_resultante << "INT: ";
    if(variables[i].getTipo_variable() == "double")
      archivo_resultante << "DOUBLE: ";
    archivo_resultante << variables[i].getNombre();
    if(!variables[i].getValor().empty())
      archivo_resultante << " = " << variables[i].getValor();
    archivo_resultante << std::endl;
  }

  //Inserta los bucles, su tipo y la línea en la que aparece.
  archivo_resultante <<"\nSTATEMENTS:\n";
  for(int i = 0 ; i < bucles.size(); i++){
    archivo_resultante << "[Line " << bucles[i].getLinea() << " ] LOOP: ";
    if(bucles[i].getTipo_bucle() == "for")
      archivo_resultante << "for";
    if(bucles[i].getTipo_bucle() == "while")
      archivo_resultante << "while";
    archivo_resultante << std::endl;
  }

  //Inserta si el main aparece o no.
  archivo_resultante <<"\nMAIN: \n";
  if(existe_main_){
    archivo_resultante << "True" << std::endl;
  } else{
    archivo_resultante << "False" << std::endl; 
  }

  //Inserta los comentarios junto a su contenido y línea.
  archivo_resultante << "\nCOMMENTS :\n";
  for(int i = 0; i < comentarios_simples.size();i++){
    archivo_resultante <<"[Line " << comentarios_simples[i].getLineaInicio() << " ] ";
    archivo_resultante << comentarios_simples[i].getContenido_comentario() << std::endl;
  }
  if(!comentarios_extendidos.empty()){
    archivo_resultante << "[Line " << comentarios_extendidos[0].getLineaInicio() << " - " 
    << comentarios_extendidos[0].getLineaFinal() << " ] DESCRIPTION" << std::endl;
  }

  //Inserta los cases junto a su línea y contenido.
  archivo_resultante << "\nCASES:\n";
  for(int i = 0; i < cases.size(); i++){
    archivo_resultante << "[Line " << cases[i].getNumero_linea() << "] case: " << cases[i].getValor() << std::endl;
  }

  //Cierra el archivo de salida.
  archivo_resultante.close();
}