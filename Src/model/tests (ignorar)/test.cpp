#include "NodoRed.hpp"
#include <iostream>

class NodoDummy : public NodoRed
{
public:
    NodoDummy(int id, double be, double sc) : NodoRed(id, be, sc) {}
    double calcularExcedente() override { return getBalanceEnergia(); }
};

int main()
{
    NodoDummy n(1, 10.5, 200.0);
    std::cout << "Id: " << n.getId() << "\n";
    std::cout << "Balance: " << n.getBalanceEnergia() << "\n";
    std::cout << "Saldo: " << n.getSaldoCuenta() << "\n";
    std::cout << "Excedente: " << n.calcularExcedente() << "\n";
}