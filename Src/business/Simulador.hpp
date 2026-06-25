#ifndef SIMULADOR_HPP
#define SIMULADOR_HPP

#include "GridManager.hpp"
#include "LectorCSV.hpp"

#include <filesystem>
namespace fs = std::filesystem;

#include <vector>
#include <algorithm>

class Simulador
{
private:
    GridManager grid;
    LectorCSV lector;
    
    std::vector<fs::path> obtenerArchivosCSV();

public:
    void ejecutar();    
};

std::vector<fs::path> Simulador::obtenerArchivosCSV()
{
    /*
    "fs::path" es un objeto que representa la ruta en donde se almacenan los 
    archivos que se van a utilizar en el código. Además ofrece herramientas
    cómodas para trabajar con rutas de archivos.
    */
    fs::path carpeta = "Data";
    
    std::vector<fs::path> archivos;

    /*
    "fs::directory_iterator" lo que hace es literalmente establecer un iterador
    dentro de la carpeta, recorriendo todos los elementos que tiene la misma de 
    forma secuencial. En este caso se quiere filtrar los archivos que tengan
    extension ".csv", por lo tanto en el vector se irán guardando las rutas de
    los archivos
    */
    for(const auto& entrada : fs::directory_iterator(carpeta))
    {
        if(entrada.path().extension() == ".csv")
        {
            archivos.push_back(entrada.path());
        }        
    }

    //por las dudas los ordena de menor a mayor (no es una operación cara)
    std::sort(archivos.begin(), archivos.end());

    return archivos;
}

void Simulador::ejecutar()
{
    LectorCSV lector;
    GridManager grid;

    //archivos es un vector que contiene todos los path a los archivos ".csv"
    auto archivos = obtenerArchivosCSV();

    //ahora se recorrería
    for(const auto& pathArchivo : archivos)
    {
        //se obtienen las ordenes del archivo
        auto ordenes = lector.leerOrdenes(pathArchivo);

        grid.procesarTick(ordenes);
    }
}

#endif