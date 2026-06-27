#ifndef NODOCONSUMIDOR_CPP
#define NODOCONSUMIDOR_CPP

#include "NodoConsumidor.hpp"

/*
si un consumidor tiene excedente negativo no significa que tiene energía nega-
-tiva, significa que tiene que adquirir la esa cantidad de energía para reponer
lo que consumió
*/
double NodoConsumidor::calcularExcedente()
{
    return (-consumo);
}

/*
setter/getters y métodos que faltan
*/

#endif