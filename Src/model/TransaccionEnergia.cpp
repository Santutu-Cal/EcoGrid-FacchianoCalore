#include "TransaccionEnergia.hpp"

void TransaccionEnergia::log() const
{
    std::cout << "\n---> Transacción completada <---" << std::endl;

    std::cout << "-idVendedor: " << this->idVendedor 
    << "\n-idComprador: " << this->idComprador << "\n-" << this->kwh 
    << " kWh vendidos a " << this->precio << " créditos.\n" 
    << "-Horario de transacción: " << this->hora;
}