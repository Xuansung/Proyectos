#include "automata_finito.h"
#include <fstream>
#include <sstream>
#include <iostream>

AutomataFinito::AutomataFinito() : estado_arranque_(-1), num_estados_(0) {}

void AutomataFinito::setAlfabeto(const std::set<char>& alfabeto) {
    alfabeto_ = alfabeto;
}

void AutomataFinito::setEstadoArranque(int estado_arranque) {
    estado_arranque_ = estado_arranque;
}

void AutomataFinito::AnadirEstados(const Estado& estado) {
    estados_[estado.getIdentificacion()] = estado;
    if (estado.getAceptado()) {
        estados_aceptados_.insert(estado.getIdentificacion());
    }
}

void AutomataFinito::AnadirTransiciones(int estado_origen, int estado_destino, char simbolo) {
    if (alfabeto_.find(simbolo) == alfabeto_.end()) {
        std::cout << "El símbolo no pertenece al alfabeto \n";
        std::exit(EXIT_FAILURE);
    }
    transiciones_[{estado_origen, simbolo}] = estado_destino;
}

bool AutomataFinito::LeerArchivo(const std::string& nombre_archivo) {
    std::ifstream archivo(nombre_archivo);
    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir el archivo";
        return false;
    }
    std::string linea;
    int num_linea = 1;
    while (getline(archivo, linea)) {
        // Leer el alfabeto
        if (num_linea == 1) {
            for (char simbolo : linea) {
                if (simbolo != ' ') {
                    // Si existe el símbolo '&' en el alfabeto devuelve false.
                    if (simbolo == '&') {
                        std::cerr << "Este símbolo no puede pertenecer a un alfabeto \n ";
                        return false;
                    }
                    alfabeto_.insert(simbolo);
                }
            }
        }
        // Leer el número de estados.
        else if (num_linea == 2) {
            num_estados_ = std::stoi(linea);
        }
        // Leer cuál es el estado de arranque
        else if (num_linea == 3) {
            int id_estado_arranque = std::stoi(linea);
            setEstadoArranque(id_estado_arranque);
        }
        // Leer los estados y transiciones
        else if (num_linea > 3 && num_linea <= 3 + num_estados_) {
            std::istringstream linea_estados(linea);
            int id_estado, aceptado, num_transiciones;
            if (!(linea_estados >> id_estado >> aceptado >> num_transiciones)) {
                std::cerr << "Error leyendo la definición del estado. \n";
                return false;
            }
            if (id_estado < 0 || id_estado >= num_estados_) {
                std::cerr << "El estado " << id_estado << " no es válido. \n";
                return false;
            }
            bool es_aceptado = (aceptado == 1);
            Estado estado(id_estado, es_aceptado);
            for (int i = 0; i < num_transiciones; i++) {
                char simbolo_transicion;
                int id_estado_destino;
                if (!(linea_estados >> simbolo_transicion >> id_estado_destino)) {
                    std::cerr << "Error leyendo las transiciones del estado " << id_estado << ". \n";
                    return false;
                }
                if (id_estado_destino < 0 || id_estado_destino >= num_estados_) {
                    std::cerr << "El estado destino " << id_estado_destino << " no es válido. \n";
                    return false;
                }
                // Agregar la transición
                AnadirTransiciones(id_estado, id_estado_destino, simbolo_transicion);
                estado.AnadirTransicion(simbolo_transicion, id_estado_destino);
            }
            AnadirEstados(estado);
        }
        num_linea++;
    }
    archivo.close();
    return true;
}

bool AutomataFinito::SimularCadenas(const std::string& cadena) {
    int estado_actual = estado_arranque_;

    for (char simbolo : cadena) {
        if (alfabeto_.find(simbolo) == alfabeto_.end()) {
            return false;
        }

        std::pair<int, char> clave(estado_actual, simbolo);
        auto iteracion = transiciones_.find(clave);
        if (iteracion == transiciones_.end()) {
            return false;
        }

        estado_actual = iteracion->second;
    }

    if (estados_aceptados_.find(estado_actual) != estados_aceptados_.end()) {
        return true;
    } else {
        return false;
    }
}
