#include "TransaccionEnergia.hpp"

void TransaccionEnergia::log() const
{
    auto tiempo =
        std::chrono::system_clock::to_time_t(this->timestamp);

    std::cout << "\n---> Transacción completada <---" << std::endl;

    std::cout << "-idVendedor: " << this->idVendedor 
    << "\n-idComprador: " << this->idComprador << "\n-" << this->kwh 
    << " kWh vendidos a " << this->precio << " créditos.\n" 
    << "-Horario de transacción: " << std::ctime(&tiempo) << std::endl;
}