#include "NodoRed.hpp"

#include <stdexcept>

//Constructor
NodoRed::NodoRed(int id, double balanceEnergia, double saldoCuenta):id(id), 
balanceEnergia(balanceEnergia), saldoCuenta(saldoCuenta){} 

//getters
int NodoRed::getId() const
{
    return this->id;
}

double NodoRed::getBalanceEnergia() const
{
    return this->balanceEnergia;
}

double NodoRed::getSaldoCuenta() const
{
    return this->saldoCuenta;
}

void NodoRed::setBalanceEnergia(double kwh)
{
    this->balanceEnergia = kwh;
}