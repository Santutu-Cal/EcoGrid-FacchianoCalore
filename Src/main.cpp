#ifndef MAIN_CPP
#define MAIN_CPP

#include "Simulador.hpp"

//usar para hacer el log de la ubicacion del directorio
#include <filesystem>

int main(void)
{
    /*
    esta linea es para depurar, (muestra el directorio desde el que se está
    ejecutando el programa)
    */
    std::cout << "Directorio actual: "
    << std::filesystem::current_path()
    << std::endl;

    Simulador s;
    s.ejecutar();

    return 0;
}

#endif