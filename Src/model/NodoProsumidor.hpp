#ifndef NODOPROSUMIDOR_HPP
#define NODOPROSUMIDOR_HPP

#include "NodoRed.hpp"

class NodoProsumidor : public NodoRed 
{
private:
    //modelado
    double produccionActual;  
    double consumoActual;    
    
public:
    //constructor
    NodoProsumidor(int id,
    double balanceEnergia, double saldoCuenta);

    //getters
    double getConsumoActual() const;
    double getProduccionActual() const;

    //registros de lecturas (Guardan las lecturas del tick actual)
    void registrarConsumo(double consumo);
    void registrarProduccion(double produccion);

    //el resultado de la resta será positivo si hay excedente
    double calcularExcedente() const override;
};

#endif