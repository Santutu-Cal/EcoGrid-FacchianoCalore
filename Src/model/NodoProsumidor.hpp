#ifndef NODOPROSUMIDOR_HPP
#define NODOPROSUMIDOR_HPP

#include "NodoRed.hpp"

class NodoProsumidor : public NodoRed 
{
private:
    //Modelado ficticio
    double produccion;  
    double consumo;    
    
public:
    //El resultado de la resta será positivo si hay excedente
    double calcularExcedente() override
    {
        return produccion - consumo;
    }
};

#endif