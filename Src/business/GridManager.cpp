#include "GridManager.hpp"

int GridManager::getIdOrdenBateria()
{
    return this->idOrdenBateria;
}

void GridManager::setIdOrdenBateria(int id)
{
    this->idOrdenBateria = id; 
}

void GridManager::cargarLibroDeOrdenes(const std::vector<Orden> &ordenes)
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
1) Refrescar las estructuras de datos
2) Tomár la mejor orden de compra y venta.
3) Generár la transacción (si hay matching).
4) Actualizár los kWh remanentes.
5) Eliminár las órdenes que quedaron completas.
6) Si una cola de un determinado precio queda vacía, eliminar esa entrada.
7) Volvér a evaluar la condición del while.
*/
void GridManager::inyectarBateriaEnMatching(
    NodoAlmacenamiento& bateria, double precioBaseHorario)
{
    /*
    si la cantidad de kwh de la bateria es mayor a 0 podrá ofertarlos en el 
    matching, por lo tanto se debe añadir al libro de ordenes
    */
    if(bateria.getBalanceEnergia() > 0)
    {
        //crea un struct Orden para poder almacenarlo en el libro de ordenes
        Orden ordenBateria 
        {
            //decrementar para generar un id de orden para la bateria
            this->idOrdenBateria--,
            //false porque es una venta
            false,
            bateria.getId(),
            bateria.getBalanceEnergia(),
            precioBaseHorario
        };
        
        //carga la oferta de la bateria en bidMap
        this->askMap[ordenBateria.precio].push(ordenBateria);
    }
}


void GridManager::procesarTick(
    NodoAlmacenamiento& bateria, 
    const std::vector<Orden> &ordenes, 
    const std::string hora, 
    CapaDatos& cp
)
{
    //limpiar estructuras de datos para que no quede basura de anteriores ticks
    this->bidMap.clear();
    this->askMap.clear();

    //carga bidMap y askMap de ordenes correspondientes a cada uno
    this->cargarLibroDeOrdenes(ordenes);

    //obtener precio base segun horario (utilizado en interacciones de la bateria)
    double precioBaseHorario = cp.obtenerPrecioBase(hora);

    inyectarBateriaEnMatching(bateria, precioBaseHorario);

    //inicializar contador de transacciones completas por tick
    this->transaccionesCompletadas = 0;

    //mientras los dos mapas contengan ordenes (comienza el matching)
    while(!this->bidMap.empty() && !this->askMap.empty())
    {
        /*
        toma de bidMap el mejor precio de compra y toma de askMap el mejor 
        precio de venta (iteradores)
        */
        auto mejorBid = this->bidMap.begin();
        auto mejorAsk = this->askMap.begin();
    
        //spread: bid >= ask <<-- revisa si hay match (spread)
        if(mejorBid->first >= mejorAsk->first)
        {
            /*
            se usa "second.front" porque el valor es de tipo:
            queue<Orden>, por lo tanto sacar el primero que entró
            */
            Orden& ordenCompra = mejorBid->second.front();
            Orden& ordenVenta = mejorAsk->second.front();

            /*
            Aclaracion: en la transacción no se guardan montos totales, sino que
            se guardan precios unitarios (precio por kwh). GridManager no 
            debería de modificar saldos 
            */

            //cantidad de energia a transaccionar
            double energia = std::min(ordenCompra.kwh, ordenVenta.kwh);

            //precio de transaccion
            double precio = (ordenCompra.precio + ordenVenta.precio) / 2;
            
            //depuracion de cantidades de kwh y precio
            std::cout << "Compra: " << ordenCompra.kwh << '\n';
            std::cout << "Venta : " << ordenVenta.kwh << '\n';
            std::cout << "Energia a transaccionar: " << energia << '\n';
            std::cout << "Precio de transaccion: " << precio << '\n';
            //pronto será eliminado

            /*
            actualizar campos de energía. Del comprador se resta la cantidad
            de energía que fue satisfecha de su orden y del vendedor lógicamente
            lo que logró vender
            */
            ordenCompra.kwh -= energia;
            ordenVenta.kwh -= energia;

            //crear e inicializar objeto TransaccionEnergia
            TransaccionEnergia transaccion = {
                ordenVenta.idNodo,
                ordenCompra.idNodo,
                energia,
                precio,
                hora + ":00"
            };
            
            //añadir al vector de transaccioes del objeto la transaccion hecha
            this->transacciones.push_back(transaccion);

            //transaccion completa, contador incrementa
            this->transaccionesCompletadas++;

            //imprimir informacion de transacción
            transaccion.logTransaccion();

            /*
            este último sector busca verificar si las dos ordenes que partici - 
            - paron del matching fueron realmente satisfechas, a partir de ahi
            evaluar si se quedan o se retiran de sus respectivas estructuras de 
            datos.
            */
            double umbral = 0.001;

            mejorBid->second.pop(); //quitar orden vieja (valor sin restar kwh)
            if(ordenCompra.kwh > umbral)
            //si aún queda energía por satisfacer en la orden de compra
                mejorBid->second.push(ordenCompra);

            mejorAsk->second.pop(); //idem al anterior
            if(ordenVenta.kwh > umbral)
            //idem al anterior pero en la orden de venta
                mejorAsk->second.push(ordenVenta);
                
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

    /* 
    Consultar si quedaron vendedores en el vector askMap y hacer una transacción
    con cada uno. Esto se hace fuera del while porque mientras se esté dentro 
    del mismo todavía puede aparecer un comprador que consuma esa energía.
    */
    while(!askMap.empty())
    {
        //obtener el nivel de precio mas bajo del map
        auto ask = this->askMap.begin();

        //obtener orden de venta
        Orden& ordenVenta = ask->second.front();
        
        /*
        la energia comprada por la bateria se suma al objeto de C++ porque se 
        sabe que la misma trabaja siempre en RAM (mas allá de que también sea 
        persistida en la bdd)
        */
        bateria.setBalanceEnergia(
            bateria.getBalanceEnergia() + ordenVenta.kwh);

        TransaccionEnergia transaccionBateria
        {
            ordenVenta.idNodo,
            bateria.getId(),
            ordenVenta.kwh,
            precioBaseHorario,
            hora
        };

        //sumar la transacción al vector de transacciones a persistir
        this->transacciones.push_back(transaccionBateria);

        //eliminar esa orden de la cola
        ask->second.pop();

        //¿quedan elementos en ese indice del mapa?
        if(ask->second.empty())
        {
            //borrar precio (indice) del mapa
            this->askMap.erase(ask);
        }
    }

    //persistir transacciones en la bdd
    cp.persistirTransacciones(this->transacciones);

    this->logTick(hora);
}

void GridManager::logTick(const std::string hora) const
{
    std::cout << "\n--------> Tick completado <--------\n"
    << "-Hora transcurrida en el tick: " + hora
    << "\n-Se completaron y persisitieron " + this->transaccionesCompletadas
    << " transacciones en este tick"
    << "\n<--------------------------------->" << std::endl;
}