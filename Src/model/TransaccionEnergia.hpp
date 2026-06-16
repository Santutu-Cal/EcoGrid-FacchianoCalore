#ifndef TRANSACCIONENERGIA_HPP
#define TRANSACCIONENERGIA_HPP

#include <chrono>
#include <iostream>

typedef std::chrono::system_clock::time_point time;

typedef struct TransaccionEnergia
{
    int idVendedor;
    int idComprador;
    double kwh;
    double precio;
    time timestamp;

    /*
    TransaccionEnergia(int idV, int idC, double k, double p, time t) :
        idVendedor{idV}, idComprador{idC}, kwh{k}, precio{p}, timestamp{t} {}
    */
    
    void imprimirLog() const
    {
        std::cout << "\n---> Transacción completada <---" << std::endl;

        std::cout << "\n-idVendedor: " << this->idVendedor 
        << "\n-idComprador: " << this->idComprador << "\n- " << this->kwh 
        << " kWh vendidos a " << this->precio << " créditos.\n" 
        << "-Horario de transacción: " << this->timestamp << std::endl;
    }    
}TransaccionEnergia;

#endif