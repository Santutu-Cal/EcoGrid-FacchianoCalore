#ifndef NODOCONSUMIDOR_HPP
#define NODOCONSUMIDOR_HPP

#include "NodoRed.hpp"

enum PerfilConsumo {Residencial, Comercial, Industrial};

class NodoConsumidor : public NodoRed
{
private:
    PerfilConsumo perfil;
    double consumo;

public:
    double calcularExcedente() override;
};

#endif