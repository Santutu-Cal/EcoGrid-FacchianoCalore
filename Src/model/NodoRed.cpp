#ifndef NODORED_CPP
#define NODORED_CPP

#include "NodoRed.hpp"

#include <stdexcept>

//getters
int NodoRed::getId() const
{
    return id;
}

std::string NodoRed::getUbicacion() const
{
    return ubicacion;
}

double NodoRed::getBalanceEnergia() const
{
    return balanceEnergia;
}

double NodoRed::getSaldoCuenta() const
{
    return saldoCuenta;
}

#endif