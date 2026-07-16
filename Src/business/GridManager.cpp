#include "GridManager.hpp"

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
void GridManager::procesarTick
(NodoAlmacenamiento bateria, const std::vector<Orden> &ordenes, 
    const std::string hora)
{
    /*
    limpio las estructuras de datos para que no quede basura de anteriores 
    ticks, cosa de que los siguientes procesamiento sean limpios
    */
    this->bidMap.clear();
    this->askMap.clear();

    //carga bidMap y askMap de ordenes correspondientes a cada uno
    cargarLibroDeOrdenes(ordenes);

    /*
    obtener desde la tabla config_tarifas, se usará dos veces dentro de 
    procesarTick()
    */
    double precioBaseHorario = CapaDatos.obtenerPrecioBase(hora);

    /*
    si la cantidad de kwh de la bateria es mayor a 0 podrá ofertarlos en el 
    matching, por lo tanto se debe añadir al libro de ordenes
    */
    if(bateria.getBalanceEnergia() > 0)
    {
        //crea un struct Orden para poder almacenarlo en el libro de ordenes
        Orden orden_bateria(
            this->id_orden_bateria--,
            false,
            bateria.getId(),
            bateria.getBalanceEnergia(),
            precioBaseHorario
        );
        
        //carga la oferta de la bateria en bidMap
        this->askMap[orden_bateria.precio].push(orden_bateria);
    }

    //mientras los dos mapas contengan ordenes
    while(!this->bidMap.empty() && !this->askMap.empty())
    {
        /*
        toma de bidMap la mejor oferta de compra y toma la mejor oferta de 
        venta de askMap (ordenes)
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
            Orden &ordenCompra = mejorBid->second.front();
            Orden &ordenVenta = mejorAsk->second.front();

            /*
            Aclaracion: en la transacción no se guardan montos totales, sino que
            se guardan precios unitarios (precio por kwh). GridManager no 
            debería de modificar saldos 
            */

            //cantidad de energia transaccionada
            double energia = std::min(ordenCompra.kwh, ordenVenta.kwh);

            //precio de transaccion
            double precio = (ordenCompra.precio + ordenVenta.precio) / 2;
            
            //depuracion de cantidades de kwh y precio
            std::cout << "Compra: " << ordenCompra.kwh << '\n';
            std::cout << "Venta : " << ordenVenta.kwh << '\n';
            std::cout << "Energia: " << energia << '\n';
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

            //imprimir informacion de transacción
            transaccion.log();

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
    Aca iría la transferencia de excedentes de energía de la transaccion. Habría
    que consultar si quedaron vendedores en el vector askMap, reunir la energía
    total que tienen y pasársela al NodoAlmacenamiento.
    Esto se hace fuera del while porque mientras se esté dentro del mismo 
    todavía puede aparecer un comprador que consuma esa energía.
    */
    while(!askMap.empty())
    {
        //obtener el nivel de precio mas bajo del map
        auto it = this->askMap.begin();

        //obtener orden de venta
        Orden &ordenVenta = it->second.front();
        
        /*
        la energia comprada por la bateria se suma al objeto de C++ porque se 
        sabe que la misma trabaja siempre en RAM (mas allá de que también sea 
        persistida en la bdd)
        */
        bateria.balanceEnergia += ordenVenta.kwh;

        TransaccionEnergia transaccionBateria = (
            ordenVenta.idNodo,
            bateria.getId(),
            ordenVenta.kwh,
            precioBaseHorario,
            hora
        );

        //sumar la transacción al vector de transacciones a persistir
        this->transacciones.push_back(transaccionBateria);

        //eliminar esa orden de la cola
        it->second.pop();

        //¿quedan elementos en ese indice del mapa?
        if(it->second.empty())
        {
            //borrar precio (indice) del mapa
            this->askMap.erase();
        }
    }

    /*
    ya por último iría la persistencia de las transacciones en la bdd
    */
   CapaDatos.persistirTransacciones(this->transacciones);
}
