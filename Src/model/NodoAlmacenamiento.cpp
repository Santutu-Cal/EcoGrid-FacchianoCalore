#include "NodoAlmacenamiento.hpp"

//Constructor
NodoAlmacenamiento::NodoAlmacenamiento(int id,
double balanceEnergia, double saldoCuenta) 
: NodoRed(id, balanceEnergia, saldoCuenta) {};

//Polimorfismo
double NodoAlmacenamiento::calcularExcedente() 
{
    return this->getBalanceEnergia();
}