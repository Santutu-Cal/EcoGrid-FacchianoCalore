#ifndef ORDEN_HPP
#define ORDEN_HPP

#include <cstdint>

typedef struct Orden
{
    int idOrden;
    bool esCompra;
    int idNodo;
    double precio;
    double kwh;

    /*
    "unsigned integer 64 bits", el uso simplemente se limita a "tengo un numero
    entero grande donde voy a guardar algo". En este caso se toma como un
    numero el cual refiere a la posición que tiene el orden en esa secuencia de 
    ordenes que fueron llegando como entrada al programa. Se le dice timestamp
    porque cumple con la propiedad de si una orden A llegó antes que otra
    orden B, timestamp(A) < timestamp(B).
    Además su uso se debe a que nunca será negativo, lo cual en tiene sentido si
    se usará una variable incremental.
    */
    uint64_t secuencia;
}Orden;

#endif