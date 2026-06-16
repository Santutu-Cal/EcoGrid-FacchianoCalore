#ifndef GRIDMANAGER_HPP
#define GRIDMANAGER_HPP

#include "LectorCSV.hpp"
#include "Orden.hpp"
#include "TransaccionEnergia.hpp"

#include <map>
#include <queue>
#include <vector>

typedef std::map<double, std::queue<Orden>, std::greater<double>> bMap;
typedef std::map<double, std::queue<Orden>> aMap; 

class GridManager
{
private:
    std::vector<Orden> ordenes;
    bMap bidMap; 
    aMap askMap; 

public:
    void cargarLibroDeOrdenes(const std::string& nombreArchivo);
    void procesarTick(const std::string& nombreArchivo);
    /*
    en la praćtica procesarTick le pasará como argumento el nombre del archivo a
    cargarLibroDeOrdenes.
    */
};

//implementación de métodos
void GridManager::cargarLibroDeOrdenes(const std::string& nombreArchivo)
{
    //leer archivo .csv y obtener el vector<Orden> ordenes
    LectorCSV lector;
    this->ordenes = lector.leerOrdenes(nombreArchivo);

    for(const auto& orden : this->ordenes)
    {
        if(orden.esCompra)
        {
            double precio = orden.precio;
            this->bidMap[precio].push(orden);
        }
        else
        {
            double precio = orden.precio;
            this->askMap[precio].push(orden);
        }
    }
}

/*
procesarTick:
Tomár la mejor orden de compra y venta.
Generár la transacción (si hay matching).
Actualizár los kWh remanentes.
Eliminár las órdenes que quedaron completas.
Si una cola de un determinado precio queda vacía, eliminár esa entrada del mapa.
Volvér a evaluar la condición del while.
*/
void GridManager::procesarTick(const std::string& nombreArchivo)
{
    /*
    limpio las estructuras de datos para que no quede basura de anteriores ticks
    */
    this->bidMap.clear();
    this->askMap.clear();

    cargarLibroDeOrdenes(nombreArchivo);

    while(!this->bidMap.empty() && !this->askMap.empty())
    {
        auto mejorBid = this->bidMap.begin();
        auto mejorAsk = this->askMap.begin();

        //spread = bid >= ask
        if(mejorBid->first >= mejorAsk->first)
        //si hay match
        {
            /*
            se usa "second.front" porque el valor es de tipo:
            queue<Orden>
            */
            Orden ordenCompra = mejorBid->second.front();
            Orden ordenVenta = mejorAsk->second.front();

            //iniciar transaccion
            double energia = std::min(ordenCompra.kwh, ordenVenta.kwh);
            double precio = (ordenCompra.precio + ordenVenta.precio) / 2;
            
            //actualizar
            ordenCompra.kwh -= energia;
            ordenVenta.kwh -= energia;
            
            std::vector<TransaccionEnergia> transacciones;

            TransaccionEnergia transaccion = {
                ordenVenta.idNodo,
                ordenCompra.idNodo,
                energia,
                precio
            };
            
            transacciones.push_back(transaccion);

            //acá iría la persistencia (llamada a CapaDatos)

            transaccion.imprimirLog();

            double umbral = 0.001;
            if(ordenCompra.kwh > umbral)
            //si aún queda energía por satisfacer en la orden (cliente)
                mejorBid->second.push(ordenCompra);
            else
            //si no, el cliente se retira de la estructura de datos
                mejorBid->second.pop();

            if(ordenVenta.kwh > umbral)
            //si aún queda energía por satisfacer en la orden (vendedor)
                mejorAsk->second.push(ordenVenta);
            else
            //si no, el vendedor se retira de la estructura de datos
                mejorAsk->second.pop();
                
            //limpiar colas vacías en el mapa
            if(mejorBid->second.empty()) 
                bidMap.erase(mejorBid);
            if(mejorAsk->second.empty()) 
                askMap.erase(mejorAsk);
        }
        //si no hay match
        else
        {
            break;
        }
    }
}

#endif