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
    std::vector<TransaccionEnergia> transacciones;
    bMap bidMap; 
    aMap askMap; 

    void cargarLibroDeOrdenes(std::vector<Orden> ordenes);
public:
    void procesarTick(std::vector<Orden> ordenes);
    /*
    en la praćtica procesarTick le pasará como argumento el nombre del archivo a
    cargarLibroDeOrdenes.
    */
};

void GridManager::cargarLibroDeOrdenes(std::vector<Orden> ordenes)
{
    for(const auto& orden : ordenes)
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
1) Tomár la mejor orden de compra y venta.
2) Generár la transacción (si hay matching).
3) Actualizár los kWh remanentes.
4) Eliminár las órdenes que quedaron completas.
5) Si una cola de un determinado precio queda vacía, eliminar esa entrada.
6) Volvér a evaluar la condición del while.
*/
void GridManager::procesarTick(std::vector<Orden> ordenes)
{
    /*
    limpio las estructuras de datos para que no quede basura de anteriores 
    ticks, cosa de que los siguientes procesamiento sean limpios
    */
    this->bidMap.clear();
    this->askMap.clear();

    /*
    Consulta a la bdd para meter al nodoBateria en askMap()
    (CapaDatos)
    */

    cargarLibroDeOrdenes(ordenes);

    while(!this->bidMap.empty() && !this->askMap.empty())
    {
        auto mejorBid = this->bidMap.begin();
        auto mejorAsk = this->askMap.begin();
    
        //spread: bid >= ask
        if(mejorBid->first >= mejorAsk->first)
        //si hay match
        {
            /*
            se usa "second.front" porque el valor es de tipo:
            queue<Orden>
            por lo tanto sacar el primero que entró
            */
            Orden ordenCompra = mejorBid->second.front();
            Orden ordenVenta = mejorAsk->second.front();

            //iniciar transaccion
            //cantidad de energia transaccionada
            double energia = std::min(ordenCompra.kwh, ordenVenta.kwh);

            //precio de transaccion
            double precio = (ordenCompra.precio + ordenVenta.precio) / 2;
            
            /*
            actualizar campos de energía. Del comprador se resta la cantidad
            de energía que fue satisfecha de su orden y del vendedor lógicamente
            lo que logro vender
            */
            ordenCompra.kwh -= energia;
            ordenVenta.kwh -= energia;
            
            //crear objeto TransaccionEnergia
            TransaccionEnergia transaccion;

            //tomar tiempo actual (sistema operativo)
            time t = std::chrono::system_clock::now(); 

            TransaccionEnergia transaccion = {
                ordenVenta.idNodo,
                ordenCompra.idNodo,
                energia,
                precio,
                t
            };
            
            this->transacciones.push_back(transaccion);

            /*
            * acá iría la persistencia (CapaDatos)
            *
            * 
            * 
            * 
            */

            //imprimir transacción
            transaccion.log();

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