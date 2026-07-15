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

    /*
    TransaccionEnergia(int idV, int idC, double k, double p, time t) :
        idVendedor{idV}, idComprador{idC}, kwh{k}, precio{p}, timestamp{t} {}
    */    

    void log() const;
}TransaccionEnergia;

#endif