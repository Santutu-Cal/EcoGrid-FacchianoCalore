#include <iostream>
#include <memory>
#include <vector>
#include <cassert>
#include "NodoConsumidor.hpp"

// Traduce el enum a texto para que se lea mejor en consola
std::string perfilToString(PerfilConsumo perfil) {
    switch (perfil) {
        case Residencial: return "Residencial";
        case Comercial:   return "Comercial";
        case Industrial:  return "Industrial";
        default:          return "Desconocido";
    }
}

int main() {
    std::cout << "=== Prueba de NodoConsumidor ===\n\n";

    // --- 1) Construccion y getters propios de NodoConsumidor ---
    NodoConsumidor c1(1, 0.0, 1000.0, Residencial);

    std::cout << "-- Consumidor 1 --\n";
    std::cout << "id: " << c1.getId() << "\n";                       // heredado de NodoRed
    std::cout << "balanceEnergia: " << c1.getBalanceEnergia() << "\n"; // heredado de NodoRed
    std::cout << "saldoCuenta: " << c1.getSaldoCuenta() << "\n";       // heredado de NodoRed
    std::cout << "perfil: " << perfilToString(c1.getPerfil()) << "\n";
    std::cout << "consumoActual inicial: " << c1.getConsumoActual() << "\n";
    std::cout << "excedente inicial: " << c1.calcularExcedente() << "\n\n";

    assert(c1.getId() == 1);
    assert(c1.getConsumoActual() == 0.0);
    assert(c1.calcularExcedente() == 0.0);

    // --- 2) registrarConsumo() y efecto en calcularExcedente() ---
    c1.registrarConsumo(15.5);
    std::cout << "Despues de registrarConsumo(15.5):\n";
    std::cout << "consumoActual: " << c1.getConsumoActual() << "\n";
    std::cout << "excedente (debe ser -15.5): " << c1.calcularExcedente() << "\n\n";
    assert(c1.getConsumoActual() == 15.5);
    assert(c1.calcularExcedente() == -15.5);

    // Un segundo registro debe pisar (no acumular) el valor anterior
    c1.registrarConsumo(3.0);
    std::cout << "Despues de registrarConsumo(3.0) [debe pisar el valor anterior]:\n";
    std::cout << "consumoActual: " << c1.getConsumoActual() << "\n";
    std::cout << "excedente (debe ser -3.0): " << c1.calcularExcedente() << "\n\n";
    assert(c1.getConsumoActual() == 3.0);
    assert(c1.calcularExcedente() == -3.0);

    // --- 3) Otros perfiles ---
    NodoConsumidor c2(2, 0.0, 5000.0, Industrial);
    c2.registrarConsumo(230.0);
    std::cout << "-- Consumidor 2 (" << perfilToString(c2.getPerfil()) << ") --\n";
    std::cout << "excedente (debe ser -230.0): " << c2.calcularExcedente() << "\n\n";
    assert(c2.calcularExcedente() == -230.0);

    NodoConsumidor c3(3, 0.0, 0.0, Comercial);
    std::cout << "-- Consumidor 3 (" << perfilToString(c3.getPerfil()) << ") sin registrar consumo --\n";
    std::cout << "excedente (debe ser 0.0): " << c3.calcularExcedente() << "\n\n";
    assert(c3.calcularExcedente() == 0.0);

    // --- 4) Polimorfismo: usar NodoConsumidor a traves de un puntero a NodoRed ---
    std::cout << "-- Prueba de polimorfismo (NodoRed*) --\n";
    std::vector<std::unique_ptr<NodoRed>> nodos;
    nodos.push_back(std::make_unique<NodoConsumidor>(4, 0.0, 200.0, Residencial));
    static_cast<NodoConsumidor*>(nodos[0].get())->registrarConsumo(42.0);

    for (auto& n : nodos) {
        std::cout << "Nodo id=" << n->getId()
                  << " excedente=" << n->calcularExcedente() << "\n";
    }
    assert(nodos[0]->calcularExcedente() == -42.0);

    std::cout << "\n=== Todas las pruebas pasaron correctamente ===\n";
    return 0;
}