#ifndef TRANSACCIONENERGIA_HPP
#define TRANSACCIONENERGIA_HPP

#include <chrono>
#include <iostream>

struct TransaccionEnergia
{
    int idVendedor = 0;
    int idComprador = 0;
    double kwh = 0.0;
    double precio = 0.0;
    int hora = 0;

    void logTransaccion() const;
};

#endif