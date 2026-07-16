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
    std::string hora;  

    void log() const;
}TransaccionEnergia;

#endif