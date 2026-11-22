#ifndef ORDEN_HPP
#define ORDEN_HPP

#include <cstdint>

struct Orden
{
    int idOrden = 0;
    bool esCompra = false;
    int idNodo = 0;
    double kwh = 0.0;
    double precio = 0.0;
};

#endif