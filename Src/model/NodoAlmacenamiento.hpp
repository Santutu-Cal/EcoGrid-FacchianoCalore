#ifndef NODOALMACENAMIENTO_HPP
#define NODOALMACENAMIENTO_HPP

#include "NodoRed.hpp"

class NodoAlmacenamiento : public NodoRed {
    double cargaActual;
    double capacidad;

public:
    //Constructor
    NodoAlmacenamiento(int id,
    double balanceEnergia, double saldoCuenta,
    double cargaActual, double capacidad);

    //Getters
    double getCargaActual() const;
    double getCapacidad() const;

    //Funcionalidad de la bateria
    void absorber(double energia);
    void ofrecer(double energia);

    //Polimorfismo
    double calcularExcedente() const override;
};

#endif