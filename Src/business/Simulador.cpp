#include "Simulador.hpp"

void Simulador::ejecutar()
{
    LectorCSV lector;
    GridManager grid;
    CapaDatos cp;

    //conectar bdd y cargar nodos de la misma (inicializándolos)
    cp.conectar();
    this->nodos = cp.obtenerNodos();

    //acceder a archivos secuencialmente, leer las ordenes y procesarlas
    for(const auto& archivoCSV : this->archivos)
    {
        //log de archivo en proceso
        std::cout << "\n" << archivoCSV << std::endl;

        //obtener la hora del archivo para pasársela a procesarTick
        const std::string hora = archivoCSV.substr(8,2);

        //obtiene las ordenes del archivo
        auto ordenes = lector.leerOrdenes(archivoCSV);
        
        //procesa las ordenes obtenidas del archivo
        grid.procesarTick(ordenes, hora);
    }
}