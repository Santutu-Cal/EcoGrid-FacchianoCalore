#include "ConfigReader.hpp"

const std::string ConfigReader::leerConfig(const std::string& configIni)
{
    std::ifstream archivoConfig(configIni);

    if(!archivoConfig)
    {
        std::cout << "Desde ConfigReader.cpp" << std::endl; 
        throw 
        std::runtime_error("*No se pudo abrir el archivo: " + configIni); 
    }

    std::string conexion;
    std::string linea;
    while(std::getline(archivoConfig, linea))
    {
        conexion += linea + ' ';

    }

    return conexion;

}

