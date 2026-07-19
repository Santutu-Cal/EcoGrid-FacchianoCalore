#include "Simulador.hpp"

void Simulador::ejecutar()
{
    //instanciar objetos necesarios para la simulación
    LectorCSV lector;
    GridManager grid;
    CapaDatos cp;

    //conectar bdd y cargar nodos de la misma (inicializándolos)
    cp.conectar();
    this->nodos = cp.obtenerNodos();
    
    //inicializar el id de las ordenes para la bateria
    grid.setIdOrdenBateria(0);

    //encontrar la bateria en el vector de nodos
    NodoAlmacenamiento* b = nullptr;
    for(const auto& nodo : this->nodos)
    {
        //si es NodoAlmacenamiento* accede al scope del if y lo castea a 
        if(
            //obtener puntero crudo desde unique_ptr con el metodo "get"
            NodoAlmacenamiento* b = 
                dynamic_cast<NodoAlmacenamiento*>(nodo.get())
        )
        {
            //frenar porque ya se encontró la bateria
            break;
        }    
    }

    if(!b)
    {
        throw std::runtime_error(
            "\nERROR: no se pudo encontrar un NodoAlmacenamiento\n"
        );
    }

    //referencia segura que contiene la batería
    NodoAlmacenamiento& bateria = *b;

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
        grid.procesarTick(bateria, ordenes, hora, cp);
    }
}