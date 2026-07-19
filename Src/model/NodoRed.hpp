#ifndef NODORED_HPP
#define NODORED_HPP

#include <string>

class NodoRed
{
private:
    int id;          
    double balanceEnergia;  //kWh
    double saldoCuenta;     //créditos

public:
    NodoRed(int id, double balanceEnergia, double saldoCuenta);
    virtual ~NodoRed() = default; 

    int getId() const;
    double getBalanceEnergia() const;
    double getSaldoCuenta() const;

    void setBalanceEnergia(double kwh);

    virtual double calcularExcedente() = 0;
};

#endif