#ifndef NODOALMACENAMIENTO_HPP
#define NODOALMACENAMIENTO_HPP

#define ID_BATERIA 99

#include "NodoRed.hpp"

class NodoAlmacenamiento : public NodoRed 
{
public:
    NodoAlmacenamiento(int id, double balanceEnergia, double saldoCuenta);

    //setea el balance de energia en la bateria
    void NodoAlmacenamiento::setBalanceEnergia(double kwh);

    //Polimorfismo
    double calcularExcedente() override;
};

#endif