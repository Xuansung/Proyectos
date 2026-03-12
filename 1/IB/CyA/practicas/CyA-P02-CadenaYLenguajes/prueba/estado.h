#ifndef ESTADO_H
#define ESTADO_H

#include <map>
#include <set>

class Estado {
public:
    Estado(int identificacion, bool aceptado);
    int getIdentificacion() const;
    bool getAceptado() const;
    void AnadirTransicion(char simbolo, int estado_destino);
    const std::map<char, std::set<int>>& getTransiciones() const;

private:
    int identificacion_;
    bool aceptado_;
    std::map<char, std::set<int>> transiciones_;
};

#endif
