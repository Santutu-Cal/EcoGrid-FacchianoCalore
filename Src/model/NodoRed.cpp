#include "NodoRed.hpp"

#include <stdexcept>

//Constructor
NodoRed::NodoRed(int id, double balanceEnergia, double saldoCuenta):id(id), 
balanceEnergia(balanceEnergia), saldoCuenta(saldoCuenta){} 

//getters
int NodoRed::getId() const
{
    return id;
}

double NodoRed::getBalanceEnergia() const
{
    return balanceEnergia;
}

double NodoRed::getSaldoCuenta() const
{
    return saldoCuenta;
}