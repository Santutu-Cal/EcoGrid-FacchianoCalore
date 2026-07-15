#include "NodoConsumidor.hpp"

//constructor
NodoConsumidor::NodoConsumidor(int id,
double balanceEnergia, double saldoCuenta,
PerfilConsumo perfil)
:NodoRed(id, balanceEnergia, saldoCuenta),
perfil(perfil) {};

//getters. La logica detras de cada uno se hará en el simulador o el gridmanager
PerfilConsumo NodoConsumidor::getPerfil() 
{
    return perfil;
}

/*
Si un consumidor tiene excedente negativo no significa que tiene energía nega-
-tiva, significa que tiene que adquirir esa cantidad de energía para reponer
lo que consumió. 
*/
double NodoConsumidor::calcularExcedente()
{
    return -(getBalanceEnergia());
}