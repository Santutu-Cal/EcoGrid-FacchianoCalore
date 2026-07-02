#ifndef GRIDMANAGER_HPP
#define GRIDMANAGER_HPP

#include "LectorCSV.hpp"
#include "Orden.hpp"
#include "TransaccionEnergia.hpp"

#include <map>
#include <queue>
#include <vector>

typedef std::map<double, std::queue<Orden>, std::greater<double>> bMap;
typedef std::map<double, std::queue<Orden>> aMap; 

class GridManager
{
private:
    std::vector<TransaccionEnergia> transacciones;
    bMap bidMap; 
    aMap askMap; 

    void cargarLibroDeOrdenes(const std::vector<Orden>& ordenes);
    
public:
    /*
    en la praćtica procesarTick le pasará como argumento el nombre del archivo a
    cargarLibroDeOrdenes.
    */
    void procesarTick(const std::vector<Orden>& ordenes);
};

#endif