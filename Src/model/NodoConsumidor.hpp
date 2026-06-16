#ifndef NODOCONSUMIDOR_HPP
#define NODOCONSUMIDOR_HPP

#include "NodoRed.hpp"

enum PerfilConsumo {Residencial, Comercial, Industrial};

class NodoConsumidor : public NodoRed
{
private:
    PerfilConsumo perfil;

public:
    double calcularExcedente() override
    {
        switch(perfil)
        {
            //acá puse valores de ejemplo, se podrían poner valores mas acordes
            case Residencial: 
                return -5.0;
            case Comercial:
                return -20.0;
            case Industrial:
                return -50.0;          
        }
    }   
};

#endif