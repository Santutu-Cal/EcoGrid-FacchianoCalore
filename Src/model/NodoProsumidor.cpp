#include "NodoProsumidor.hpp"


//Constructor
NodoProsumidor::NodoProsumidor(int id, double balanceEnergia, 
double saldoCuenta):NodoRed(id, balanceEnergia, saldoCuenta) {};



//Polimorfismo
double NodoProsumidor::calcularExcedente()
{
    return getBalanceEnergia();
}
