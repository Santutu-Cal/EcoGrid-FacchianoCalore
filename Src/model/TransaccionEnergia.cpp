#include "TransaccionEnergia.hpp"

void TransaccionEnergia::logTransaccion() const
{
    std::cout << "\n---> Transacción completada <---\n"
    << "-idVendedor: " << this->idVendedor 
    << "\n-idComprador: " << this->idComprador << "\n-" << this->kwh 
    << " kWh vendidos a " << this->precio << " por unidad.\n" 
    << "-Horario de transacción: " << this->hora << ":00"
    << "\n<------------------------------>" << std::endl;
}