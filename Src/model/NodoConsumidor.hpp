#ifndef NODOCONSUMIDOR_HPP
#define NODOCONSUMIDOR_HPP

#include "NodoRed.hpp"

enum PerfilConsumo {Residencial, Comercial, Industrial};

class NodoConsumidor : public NodoRed
{
private:
    PerfilConsumo perfil;
    double consumoActual;

public:
    //constructor
    NodoConsumidor(int id, double balanceEnergia, double saldoCuenta,
    PerfilConsumo perfil);
    //No creamos un destructor ya que con el que hereda de nodoRed es suficiente

    //getters
    double getConsumoActual();
    PerfilConsumo getPerfil();
    //modificacion
    void registrarConsumo(double consumo);
    //polimorfismo
    double calcularExcedente() const override;
};

#endif