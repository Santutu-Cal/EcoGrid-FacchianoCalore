#ifndef NODOALMACENAMIENTO_HPP
#define NODOALMACENAMIENTO_HPP

#include "NodoRed.hpp"

class NodoAlmacenamiento : public NodoRed 
{
public:
    NodoAlmacenamiento(int id, double balanceEnergia, double saldoCuenta);

    //Polimorfismo
    double calcularExcedente() override;
};

#endif