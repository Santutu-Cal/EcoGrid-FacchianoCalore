#ifndef SIMULADOR_HPP
#define SIMULADOR_HPP

#include "GridManager.hpp"
#include "LectorCSV.hpp"

#include <vector>
#include <algorithm>

class Simulador
{
private:
    GridManager grid;
    LectorCSV lector;

public:
    void ejecutar();    
};

#endif