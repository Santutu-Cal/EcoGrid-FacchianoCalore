#include "NodoConsumidor.hpp"


//constructor
NodoConsumidor::NodoConsumidor(int id,
double balanceEnergia, double saldoCuenta,
PerfilConsumo perfil)
:NodoRed(id, balanceEnergia, saldoCuenta),
perfil(perfil), consumoActual(0.0){};

//getters. La logica detras de cada uno se hará en el simulador o el gridmanager
PerfilConsumo NodoConsumidor::getPerfil() {
    return perfil;
}
//permite consultar la ultima consulta del consumo
double NodoConsumidor::getConsumoActual(){
    return consumoActual;
}
//guarda la lectura del tick actual
void NodoConsumidor::registrarConsumo(double consumo){
    consumoActual = consumo;
}
/*
    Si un consumidor tiene excedente negativo no significa que tiene energía nega-
-tiva, significa que tiene que adquirir esa cantidad de energía para reponer
lo que consumió. 
    Como el consumidor nunca produce energia, exponente = ProduccionActual - consumoActual, es igual
a hacer excedente = 0 - consumo actual = - consumoActual
*/

double NodoConsumidor::calcularExcedente() const {
    return -consumoActual;
}