#include "NodoRed.hpp"

#include <stdexcept>

//Constructor
NodoRed::NodoRed(int id, double balanceEnergia, double saldoCuenta)
    : id(id), balanceEnergia(balanceEnergia), saldoCuenta(saldoCuenta)
{
    if (id < 0)
    {
        throw std::invalid_argument("El ID del nodo no puede ser negativo.");
    }
} 

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

void NodoRed::setSaldoCuenta(double saldo)
{
    this->saldoCuenta = saldo;
}