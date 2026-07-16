#include "Simulador.hpp"

void Simulador::ejecutar()
{
    LectorCSV lector;
    GridManager grid;
    CapaDatos cp;

    //conectar bdd y cargar nodos de la misma (inicializándolos)
    cp.conectar();
    this->nodos = cp.obtenerNodos();

    //instanciar la bateria (aún no inicializada con valores de la bdd)
    std::unique_ptr<NodoAlmacenamiento> bateria;
    
    /*
    inicializar el id de las ordenes para la bateria (será decremental con el 
    fin de distinguir las ordenes que fueron de bateria en la bdd
    */
    grid.id_orden_bateria = 0;

    for(const auto &nodo : this->nodos)
    {
        //si es NodoAlmacenamiento accede al scope del if
        if(auto bat : std::dynamic_pointer_cast<NodoAlmacenamiento>(nodo))
            //bateria con valores ya cargados, argumento para procesarTick()
            bateria = bat;
    }

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
        grid.procesarTick(bateria, ordenes, hora);
    }
}