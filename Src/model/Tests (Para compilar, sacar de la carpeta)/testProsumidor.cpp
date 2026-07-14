#include <iostream>
#include <cassert>
#include "NodoProsumidor.hpp"

int main() {
    std::cout << "=== Prueba de NodoProsumidor ===\n\n";

    // --- 1) Construccion y getters heredados ---
    NodoProsumidor p1(1, 0.0, 800.0);

    std::cout << "-- Prosumidor 1 --\n";
    std::cout << "id: " << p1.getId() << "\n";                     // heredado de NodoRed
    std::cout << "saldoCuenta: " << p1.getSaldoCuenta() << "\n";   // heredado de NodoRed
    std::cout << "consumoActual inicial: " << p1.getConsumoActual() << "\n";
    std::cout << "produccionActual inicial: " << p1.getProduccionActual() << "\n";
    std::cout << "excedente inicial (debe ser 0.0): " << p1.calcularExcedente() << "\n\n";
    assert(p1.getConsumoActual() == 0.0);
    assert(p1.getProduccionActual() == 0.0);
    assert(p1.calcularExcedente() == 0.0);

    // --- 2) registrarConsumo() ---
    p1.registrarConsumo(10.0);
    std::cout << "Despues de registrarConsumo(10.0):\n";
    std::cout << "consumoActual (debe ser 10.0): " << p1.getConsumoActual() << "\n\n";
    assert(p1.getConsumoActual() == 10.0);

    // --- 3) registrarProduccion() ---
    p1.registrarProduccion(25.0);
    std::cout << "Despues de registrarProduccion(25.0):\n";
    std::cout << "produccionActual (debe ser 25.0): " << p1.getProduccionActual() << "\n\n";
    assert(p1.getProduccionActual() == 25.0);

    // --- 4) calcularExcedente() = produccion - consumo ---
    std::cout << "excedente (produccion - consumo, debe ser 15.0): "
              << p1.calcularExcedente() << "\n\n";
    assert(p1.calcularExcedente() == 15.0);

    // --- 5) Caso: consumo mayor que produccion -> excedente negativo ---
    NodoProsumidor p2(2, 0.0, 300.0);
    p2.registrarConsumo(50.0);
    p2.registrarProduccion(20.0);
    std::cout << "-- Prosumidor 2 --\n";
    std::cout << "consumoActual: " << p2.getConsumoActual() << "\n";
    std::cout << "produccionActual: " << p2.getProduccionActual() << "\n";
    std::cout << "excedente (debe ser -30.0): " << p2.calcularExcedente() << "\n\n";
    assert(p2.calcularExcedente() == -30.0);

    // --- 6) Reasignar produccion/consumo, el ultimo valor pisa al anterior ---
    p2.registrarProduccion(80.0);
    p2.registrarConsumo(10.0);
    std::cout << "-- Prosumidor 2 tras nuevas lecturas --\n";
    std::cout << "excedente (debe ser 70.0): " << p2.calcularExcedente() << "\n";
    assert(p2.calcularExcedente() == 70.0);

    std::cout << "\n=== Todas las pruebas de NodoProsumidor pasaron correctamente ===\n";
    return 0;
}