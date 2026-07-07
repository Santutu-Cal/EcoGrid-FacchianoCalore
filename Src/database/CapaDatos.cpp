#include "CapaDatos.hpp"

//funcion de prueba
void CapaDatos::conectar()
{
    try
    {

        //obtiene las credenciales del archivo config.ini
        this->conexion = this->cr.leerConfig("Src/database/config.ini");

        //abre la conexion a la bdd
        this->sql.open( soci::postgresql, this->conexion);

        std::cout << "\n¡Conexion exitosa a la bdd!" << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << "\nDesde CapaDatos::conectar\n *Error de conexion: " 
            << e.what() << std::endl;
    }
}

