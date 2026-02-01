#include "estado.h"

Estado::Estado(int identificacion, bool aceptado)
    : identificacion_{identificacion}, aceptado_{aceptado} {}

int Estado::getIdentificacion() const {
    return identificacion_;
}

bool Estado::getAceptado() const {
    return aceptado_;
}

void Estado::AnadirTransicion(char simbolo, int estado_destino) {
    transiciones_[simbolo].insert(estado_destino);
}

const std::map<char, std::set<int>>& Estado::getTransiciones() const {
    return transiciones_;
}
