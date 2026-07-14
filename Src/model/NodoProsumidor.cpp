#include "NodoProsumidor.hpp"


//Constructor
NodoProsumidor::NodoProsumidor(int id, double balanceEnergia, 
double saldoCuenta):NodoRed(id, balanceEnergia, saldoCuenta),
produccionActual(0.0), consumoActual(0.0){};

//Getters

double NodoProsumidor::getConsumoActual() const {
    return consumoActual;
}
double NodoProsumidor::getProduccionActual() const {
    return produccionActual;
}

//Registros de lecturas

void NodoProsumidor::registrarConsumo(double consumo) {
    consumoActual = consumo;
}
void NodoProsumidor::registrarProduccion(double produccion) {
    produccionActual = produccion;
}

//Polimorfismo

double NodoProsumidor::calcularExcedente() const
{
    return produccionActual - consumoActual;
}
