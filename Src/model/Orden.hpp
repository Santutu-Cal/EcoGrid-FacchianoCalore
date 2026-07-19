#ifndef ORDEN_HPP
#define ORDEN_HPP

#include <cstdint>

typedef struct Orden
{
    int idOrden;
    bool esCompra;
    int idNodo;
    double kwh;
    double precio;
}Orden;

#endif