#ifndef SIMULADOR_HPP
#define SIMULADOR_HPP

#include "GridManager.hpp"
#include "LectorCSV.hpp"
#include "CapaDatos.hpp"

#include <vector>
#include <algorithm>

class Simulador
{
private:
    GridManager grid;
    LectorCSV lector;
    
    const std::vector<std::string> archivos{
        "Datos/ofertas_00.csv",
        "Datos/ofertas_10.csv",
        "Datos/ofertas_12.csv",
        "Datos/ofertas_14.csv",
        "Datos/ofertas_15.csv",
        "Datos/ofertas_18.csv"
    };

public:
    void ejecutar();    
};

#endif