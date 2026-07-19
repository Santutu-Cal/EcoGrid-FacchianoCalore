#ifndef GRIDMANAGER_HPP
#define GRIDMANAGER_HPP

#include "LectorCSV.hpp"

#include "Orden.hpp"

#include "TransaccionEnergia.hpp"

#include "NodoAlmacenamiento.hpp"

#include "CapaDatos.hpp"

#include <map>
#include <queue>
#include <vector>

typedef std::map<double, std::queue<Orden>, std::greater<double>> bMap;
typedef std::map<double, std::queue<Orden>> aMap; 

class GridManager
{
private:
    std::vector<TransaccionEnergia> transacciones;
    bMap bidMap; 
    aMap askMap; 

    //id para ofertas de venta de la bateria (ordenes)
    int idOrdenBateria;

    //contar cantidad de transacciones completadas en un tick
    int transaccionesCompletadas;

    //cargar las ordenes provenientes de archivoCSV al libro de órdenes
    void cargarLibroDeOrdenes(const std::vector<Orden>& ordenes);

    //cargar bateria como vendededora en el libro de ordenes (en forma de orden)
    void inyectarBateriaEnMatching
        (NodoAlmacenamiento& bateria, double precioBaseHorario);    

public:

    int getIdOrdenBateria();
    void setIdOrdenBateria(int id);

    /*
    deberá recibir como argumento:
    1) bateria (como puntero inteligente)
    2) vector de ordenes a procesar
    3) string que representa la hora del tick
    4) referencia de la conexion a la bdd
    */
    void procesarTick(
        NodoAlmacenamiento& bateria, 
        const std::vector<Orden> &ordenes, 
        int hora, 
        CapaDatos& cd
    );

    void logTick(int hora) const;
};

#endif