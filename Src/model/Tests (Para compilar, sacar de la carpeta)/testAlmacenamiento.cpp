#include <iostream>
#include <cassert>
#include "NodoAlmacenamiento.hpp"

int main() {
    std::cout << "=== Prueba de NodoAlmacenamiento ===\n\n";

    // --- 1) Construccion y getters propios ---
    // id=1, balanceEnergia=0.0, saldoCuenta=500.0, cargaActual=20.0, capacidad=100.0
    NodoAlmacenamiento bat1(1, 0.0, 500.0, 20.0, 100.0);

    std::cout << "-- Bateria 1 --\n";
    std::cout << "id: " << bat1.getId() << "\n";                       // heredado de NodoRed
    std::cout << "saldoCuenta: " << bat1.getSaldoCuenta() << "\n";     // heredado de NodoRed
    std::cout << "cargaActual: " << bat1.getCargaActual() << "\n";
    std::cout << "capacidad: " << bat1.getCapacidad() << "\n";
    std::cout << "excedente (debe ser igual a cargaActual = 20.0): "
              << bat1.calcularExcedente() << "\n\n";
    assert(bat1.getCargaActual() == 20.0);
    assert(bat1.calcularExcedente() == 20.0);

    // --- 2) absorber(): carga normal ---
    bat1.absorber(30.0);
    std::cout << "Despues de absorber(30.0):\n";
    std::cout << "cargaActual (debe ser 50.0): " << bat1.getCargaActual() << "\n\n";
    assert(bat1.getCargaActual() == 50.0);

    // --- 3) absorber(): supera la capacidad, debe quedar clamped ---
    bat1.absorber(100.0);
    std::cout << "Despues de absorber(100.0) [debe saturar en capacidad=100.0]:\n";
    std::cout << "cargaActual (debe ser 100.0): " << bat1.getCargaActual() << "\n\n";
    assert(bat1.getCargaActual() == 100.0);

    // --- 4) absorber(): valor negativo o cero, no debe hacer nada ---
    bat1.absorber(-10.0);
    std::cout << "Despues de absorber(-10.0) [no deberia cambiar nada]:\n";
    std::cout << "cargaActual (sigue en 100.0): " << bat1.getCargaActual() << "\n\n";
    assert(bat1.getCargaActual() == 100.0);

    // --- 5) ofrecer(): descarga normal ---
    bat1.ofrecer(40.0);
    std::cout << "Despues de ofrecer(40.0):\n";
    std::cout << "cargaActual (debe ser 60.0): " << bat1.getCargaActual() << "\n\n";
    assert(bat1.getCargaActual() == 60.0);

    // --- 6) ofrecer(): pide mas de lo que hay, debe entregar solo lo disponible ---
    bat1.ofrecer(1000.0);
    std::cout << "Despues de ofrecer(1000.0) [pide mas de lo disponible]:\n";
    std::cout << "cargaActual (debe quedar en 0.0): " << bat1.getCargaActual() << "\n\n";
    assert(bat1.getCargaActual() == 0.0);

    // --- 7) ofrecer(): valor negativo o cero, no debe hacer nada ---
    bat1.absorber(25.0); // recargamos un poco para probar
    bat1.ofrecer(-5.0);
    std::cout << "Recargada a 25.0 y luego ofrecer(-5.0) [no deberia cambiar nada]:\n";
    std::cout << "cargaActual (sigue en 25.0): " << bat1.getCargaActual() << "\n\n";
    assert(bat1.getCargaActual() == 25.0);

    // --- 8) calcularExcedente() siempre debe reflejar cargaActual ---
    std::cout << "excedente final (debe ser igual a cargaActual = 25.0): "
              << bat1.calcularExcedente() << "\n\n";
    assert(bat1.calcularExcedente() == bat1.getCargaActual());

    // --- 9) Segunda bateria con distinta capacidad, para probar independencia de instancias ---
    NodoAlmacenamiento bat2(2, 0.0, 0.0, 0.0, 50.0);
    bat2.absorber(200.0); // muy por encima de su capacidad
    std::cout << "-- Bateria 2 (capacidad=50.0) --\n";
    std::cout << "cargaActual tras absorber(200.0) [debe saturar en 50.0]: "
              << bat2.getCargaActual() << "\n";
    assert(bat2.getCargaActual() == 50.0);
    // Confirmamos que bat1 no se vio afectada por operaciones sobre bat2
    assert(bat1.getCargaActual() == 25.0);

    std::cout << "\n=== Todas las pruebas de NodoAlmacenamiento pasaron correctamente ===\n";
    return 0;
}