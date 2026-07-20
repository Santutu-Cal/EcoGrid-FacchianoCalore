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

    /*
    //pruebas básicas con la conexion a la bdd
    CapaDatos cd;
    cd.conectar();
    cd.insert();
    cd.select();
    cd.update();
    cd.mostrarTabla();
    */

    //empieza el programa real
    Simulador s;
    s.ejecutar();
    
  return 0;
}
