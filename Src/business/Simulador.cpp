#include "Simulador.hpp"

void Simulador::ejecutar()
{
    LectorCSV lector;
    GridManager grid;
    
    for(const auto& archivoCSV : this->archivos)
    {
        //log de archivo en proceso
        std::cout << "\n" << archivoCSV << std::endl;

        //obtiene las ordenes del archivo
        auto ordenes = lector.leerOrdenes(archivoCSV);
        
        //procesa las ordenes obtenidas del archivo
        grid.procesarTick(ordenes);
    }
}