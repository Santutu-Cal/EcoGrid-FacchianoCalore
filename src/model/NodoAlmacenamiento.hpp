#ifndef NODOALMACENAMIENTO_HPP
#define NODOALMACENAMIENTO_HPP

#include "NodoRed.hpp"

class NodoAlmacenamiento : public NodoRed 
{
public:
    NodoAlmacenamiento(int id, double balanceEnergia, double saldoCuenta);

    // Métodos específicos del nodo de almacenamiento
    void absorberEnergia(double kwh);
    void liberarEnergia(double kwh);
    double getCargaActual() const;

    //polimorfismo
    double calcularExcedente() override;
};

#endif