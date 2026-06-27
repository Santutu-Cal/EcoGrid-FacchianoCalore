#ifndef NODOPROSUMIDOR_HPP
#define NODOPROSUMIDOR_HPP

#include "NodoRed.hpp"

class NodoProsumidor : public NodoRed 
{
private:
    //modelado ficticio
    double produccion;  
    double consumo;    
    
public:
    //el resultado de la resta será positivo si hay excedente
    double calcularExcedente() override;
};

#endif