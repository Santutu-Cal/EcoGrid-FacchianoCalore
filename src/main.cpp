#include "Simulador.hpp"
#include <iostream>
#include <filesystem>

int main()
{
    std::cout << "[EcoGrid] Directorio de trabajo: "
              << std::filesystem::current_path()
              << std::endl;

    try
    {
        Simulador simulador;
        simulador.ejecutar();
        return 0;
    }
    catch(const std::exception& e)
    {
        std::cerr << "\n[EcoGrid] Terminación abrupta por excepción no recuperable: "
                  << e.what()
                  << std::endl;
        return 1;
    }
}
