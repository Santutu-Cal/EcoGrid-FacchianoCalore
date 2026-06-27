#ifndef SIMULADOR_CPP
#define SIMULADOR_CPP

#include "Simulador.hpp"

void Simulador::ejecutar()
{
    LectorCSV lector;
    GridManager grid;

    std::vector<const std::string> archivos{
        "Data/ofertas_00.csv",
        "Data/ofertas_01.csv",
        "Data/ofertas_02.csv",
        "Data/ofertas_03.csv",
        "Data/ofertas_04.csv",
        "Data/ofertas_05.csv",
        "Data/ofertas_06.csv",
        "Data/ofertas_07.csv",
        "Data/ofertas_08.csv",
        "Data/ofertas_09.csv",
        "Data/ofertas_10.csv",
        "Data/ofertas_11.csv",
        "Data/ofertas_12.csv",
        "Data/ofertas_13.csv",
        "Data/ofertas_14.csv",
        "Data/ofertas_15.csv",
        "Data/ofertas_16.csv",
        "Data/ofertas_17.csv",
        "Data/ofertas_18.csv",
        "Data/ofertas_19.csv",
        "Data/ofertas_20.csv",
        "Data/ofertas_21.csv",
        "Data/ofertas_22.csv",
        "Data/ofertas_23.csv",
        "Data/ofertas_24.csv"
    };
    
    for(const auto& archivoCSV : archivos)
    {
        //obtiene las ordenes del archivo
        auto ordenes = lector.leerOrdenes(archivoCSV);

        //procesa las ordenes obtenidas del archivo
        grid.procesarTick(ordenes);
    }
}

#endif