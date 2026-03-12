#ifndef AUTOMATA_FINITO_H
#define AUTOMATA_FINITO_H

#include "estado.h"
#include <map>
#include <set>
#include <string>

class AutomataFinito {
public:
    AutomataFinito();

    void setAlfabeto(const std::set<char>& alfabeto);
    void setEstadoArranque(int estado_arranque);
    void AnadirEstados(const Estado& estado);
    void AnadirTransiciones(int estado_origen, int estado_destino, char simbolo);
    bool LeerArchivo(const std::string& nombre_archivo);
    bool SimularCadenas(const std::string& cadena);

private:
    std::set<char> alfabeto_;
    int estado_arranque_;
    int num_estados_;
    std::map<int, Estado> estados_;
    std::set<int> estados_aceptados_;
    std::map<std::pair<int, char>, int> transiciones_;
};

#endif
