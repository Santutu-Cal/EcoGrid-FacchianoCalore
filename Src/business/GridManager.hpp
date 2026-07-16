#ifndef GRIDMANAGER_HPP
#define GRIDMANAGER_HPP

#include "LectorCSV.hpp"

#include "Orden.hpp"
#include "TransaccionEnergia.hpp"
#include "NodoAlmacenamiento.hpp"

#include "CapaDatos.hpp"

#include <map>
#include <queue>
#include <vector>

typedef std::map<double, std::queue<Orden>, std::greater<double>> bMap;
typedef std::map<double, std::queue<Orden>> aMap; 

class GridManager
{
private:
    std::vector<TransaccionEnergia>& transacciones;
    bMap bidMap; 
    aMap askMap; 

    void cargarLibroDeOrdenes(const std::vector<Orden>& ordenes);

    /*
    id apropiado para ofertas de venta de la bateria (no es id de la bateria,
    es id de la orden de la bateria, no son lo mismo)
    */
    int id_orden_bateria;

public:
    /*
    en la praćtica procesarTick le pasará como argumento el nombre del archivo a
    cargarLibroDeOrdenes. La batería una vez fue cargada desde la bdd, siempre 
    trabajará en RAM
    */
    void procesarTick
    (NodoAlmacenamiento bateria, const std::vector<Orden> &ordenes, 
        const std::string hora);
};

#endif