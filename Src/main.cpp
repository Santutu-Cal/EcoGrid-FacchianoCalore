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

    try
    {
      Simulador s;
      s.ejecutar();
      
      return 0;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Fatal error: "
        << e.what()
        << std::endl;

        return 1;
    }

  return 0;
}