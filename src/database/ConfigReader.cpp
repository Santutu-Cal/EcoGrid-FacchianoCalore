#include "ConfigReader.hpp"
#include <stdexcept>
#include <algorithm>
#include <cctype>

const std::string ConfigReader::leerConfig(const std::string& configIni)
{
    std::ifstream archivoConfig(configIni);

    if(!archivoConfig.is_open())
    {
        throw std::runtime_error("No se pudo abrir el archivo de configuración: " + configIni); 
    }

    std::string conexion;
    std::string linea;
    bool tieneContenido = false;

    while(std::getline(archivoConfig, linea))
    {
        // Quitar espacios en blanco al inicio y final
        auto inicio = linea.find_first_not_of(" \t\r\n");
        if(inicio == std::string::npos)
        {
            continue; // Línea vacía
        }

        // Ignorar comentarios que comiencen con '#' o ';'
        if(linea[inicio] == '#' || linea[inicio] == ';')
        {
            continue;
        }

        auto fin = linea.find_last_not_of(" \t\r\n");
        std::string limpia = linea.substr(inicio, fin - inicio + 1);

        conexion += limpia + ' ';
        tieneContenido = true;
    }

    if(!tieneContenido)
    {
        throw std::runtime_error("El archivo de configuración está vacío: " + configIni);
    }

    return conexion;
}

