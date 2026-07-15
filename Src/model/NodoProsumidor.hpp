#ifndef NODOPROSUMIDOR_HPP
#define NODOPROSUMIDOR_HPP

#include "NodoRed.hpp"

class NodoProsumidor : public NodoRed 
{    
public:
    //constructor
    NodoProsumidor(int id, double balanceEnergia, double saldoCuenta);

    //el resultado de la resta será positivo si hay excedente
    double calcularExcedente() override;
};

#endif