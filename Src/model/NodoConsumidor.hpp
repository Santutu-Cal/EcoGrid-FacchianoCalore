#ifndef NODOCONSUMIDOR_HPP
#define NODOCONSUMIDOR_HPP

#include "NodoRed.hpp"

enum PerfilConsumo {Residencial, Comercial, Industrial};

class NodoConsumidor : public NodoRed
{
private:
    PerfilConsumo perfil;

public:
    NodoConsumidor(int id, double balanceEnergia, double saldoCuenta,
    PerfilConsumo perfil);
    //No creamos un destructor ya que con el que hereda de NodoRed es suficiente

    //get propio del consumidor para el perfil
    PerfilConsumo getPerfil();

    //polimorfismo
    double calcularExcedente() override;
};

#endif