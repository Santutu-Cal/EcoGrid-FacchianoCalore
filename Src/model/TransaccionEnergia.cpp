#include "TransaccionEnergia.hpp"

void TransaccionEnergia::logTransaccion() const
{
    std::cout << "\n---> Transacción completada <---\n"
    << "-idVendedor: " << this->idVendedor 
    << "\n-idComprador: " << this->idComprador << "\n-" << this->kwh 
    << " kWh vendidos a " << this->precio << " créditos.\n" 
    << "-Horario de transacción: " << this->hora
    << "<-------------------->" << std::endl;
}