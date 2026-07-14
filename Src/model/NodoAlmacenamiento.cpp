#include "NodoAlmacenamiento.hpp"

//Constructor

NodoAlmacenamiento::NodoAlmacenamiento(int id,
double balanceEnergia, double saldoCuenta,
double cargaActual, double capacidad) 
: NodoRed(id, balanceEnergia, saldoCuenta),
cargaActual(cargaActual), capacidad(capacidad) {};

//Getters

double NodoAlmacenamiento::getCapacidad() const {
    return capacidad;
}

double NodoAlmacenamiento::getCargaActual() const {
    return cargaActual;
}

/*Responsabilidades de la bateria.
Como los verdaderos comportamientos se plantean en el gridmanager, 
esto solo marca lo que puede hacer la clase*/

void NodoAlmacenamiento::absorber(double energia) {

    if (energia <= 0)
        return;
    
    cargaActual += energia;
    
    if (cargaActual > capacidad)
        cargaActual = capacidad;
    
}

void NodoAlmacenamiento::ofrecer(double energia) {

    if (energia <= 0)
        return;

    if (energia > cargaActual)
        energia = cargaActual;
    
    cargaActual -= energia;
}

//Polimorfismo

double NodoAlmacenamiento::calcularExcedente() const {
    return cargaActual;
}