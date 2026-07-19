#ifndef TRANSACCIONENERGIA_HPP
#define TRANSACCIONENERGIA_HPP

#include <chrono>
#include <iostream>

typedef struct TransaccionEnergia
{
    int idVendedor;
    int idComprador;
    double kwh;
    double precio;
    int hora;

    void logTransaccion() const;
}TransaccionEnergia;

#endif