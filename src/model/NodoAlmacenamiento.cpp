#include "NodoAlmacenamiento.hpp"
#include <stdexcept>

//Constructor
NodoAlmacenamiento::NodoAlmacenamiento(int id,
double balanceEnergia, double saldoCuenta) 
: NodoRed(id, balanceEnergia, saldoCuenta) {};

void NodoAlmacenamiento::absorberEnergia(double kwh)
{
    if (kwh < 0)
    {
        throw std::invalid_argument("La energía absorbida no puede ser negativa.");
    }
    this->setBalanceEnergia(this->getBalanceEnergia() + kwh);
}

void NodoAlmacenamiento::liberarEnergia(double kwh)
{
    if (kwh < 0)
    {
        throw std::invalid_argument("La energía liberada no puede ser negativa.");
    }
    if (kwh > this->getBalanceEnergia())
    {
        throw std::runtime_error("No hay suficiente carga acumulada en la batería para liberar esa cantidad.");
    }
    this->setBalanceEnergia(this->getBalanceEnergia() - kwh);
}

double NodoAlmacenamiento::getCargaActual() const
{
    return this->getBalanceEnergia();
}

//Polimorfismo
double NodoAlmacenamiento::calcularExcedente() 
{
    return this->getBalanceEnergia();
}