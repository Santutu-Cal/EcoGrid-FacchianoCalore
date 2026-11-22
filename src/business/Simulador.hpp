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
#include <string>

class Simulador
{
private:
    GridManager grid;
    LectorCSV lector;
    CapaDatos cp;

    std::vector<std::unique_ptr<NodoRed>> nodos;

    const std::vector<std::string> archivos{
        "datos/ofertas_10.csv",
        "datos/ofertas_12.csv",
        "datos/ofertas_14.csv",
        "datos/ofertas_15.csv",
        "datos/ofertas_18.csv"
    };

    static int extraerHoraDeArchivo(const std::string& rutaArchivo);

public:
    void ejecutar();    
};

#endif
