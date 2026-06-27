#ifndef NODORED_HPP
#define NODORED_HPP

#include <string>

class NodoRed
{
private:
    int id;          
    std::string ubicacion;  //ubicación del nodo
    double balanceEnergia;  //kWh
    double saldoCuenta;     //créditos

public:
    NodoRed(int id, 
        const std::string &ubicacion, 
        double balanceEnergia, 
        double saldoCuenta
    );
    virtual ~NodoRed() = default; 

    int getId() const;
    std::string getUbicacion() const;
    double getBalanceEnergia() const;
    double getSaldoCuenta() const;

    virtual double calcularExcedente() = 0;
};

#endif