#ifndef SIMULADOR_HPP
#define SIMULADOR_HPP

#include "GridManager.hpp"
#include "LectorCSV.hpp"

#include "CapaDatos.hpp"

#include "NodoRed.hpp"
#include "NodoAlmacenamiento.hpp"

#include <vector>
#include <algorithm>
#include <stdexcept>

class Simulador
{
private:
    GridManager grid;
    LectorCSV lector;
    CapaDatos cp;

    std::vector<std::unique_ptr<NodoRed>> nodos;

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