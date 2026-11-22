#include "GridManager.hpp"
#include <iostream>
#include <algorithm>

int GridManager::getIdOrdenBateria() const
{
    return this->idOrdenBateria;
}

void GridManager::setIdOrdenBateria(int id)
{
    this->idOrdenBateria = id; 
}

void GridManager::cargarLibroDeOrdenes(const std::vector<Orden>& ordenes)
{
    for(const auto& orden : ordenes)
    {
        if(orden.esCompra)
        {
            this->bidMap[orden.precio].push(orden);
        }
        else
        {
            this->askMap[orden.precio].push(orden);
        }
    }
}

void GridManager::inyectarBateriaEnMatching(
    NodoAlmacenamiento& bateria, double precioBaseHorario)
{
    /*
    Si la batería cuenta con carga acumulada (>0), la oferta en el libro de ventas 
    a precio de referencia horario.
    */
    if(bateria.getCargaActual() > 0.0)
    {
        Orden ordenBateria 
        {
            --this->idOrdenBateria,
            false,
            bateria.getId(),
            bateria.getCargaActual(),
            precioBaseHorario
        };
        
        this->askMap[ordenBateria.precio].push(ordenBateria);
        std::cout << "[GridManager] Batería (ID " << bateria.getId() 
                  << ") inyecta oferta de " << ordenBateria.kwh 
                  << " kWh a precio " << ordenBateria.precio << "\n";
    }
}

void GridManager::procesarTick(
    NodoAlmacenamiento& bateria, 
    const std::vector<Orden>& ordenes, 
    int hora, 
    CapaDatos& cp
)
{
    // Limpiar estructuras de datos para evitar arrastre de ticks anteriores
    this->bidMap.clear();
    this->askMap.clear();
    this->transacciones.clear();
    this->transaccionesMercado = 0;
    this->transaccionesExcedente = 0;

    // Cargar órdenes del tick al libro
    this->cargarLibroDeOrdenes(ordenes);

    // Obtener precio base para la hora actual
    double precioBaseHorario = cp.obtenerPrecioBase(hora);

    // Inyectar oferta de la batería comunitaria si posee carga
    this->inyectarBateriaEnMatching(bateria, precioBaseHorario);

    constexpr double umbralMinimoKwh = 0.001;

    // Subasta doble continua: matching mientras haya cruce de precios (bid >= ask)
    while(!this->bidMap.empty() && !this->askMap.empty())
    {
        auto mejorBid = this->bidMap.begin();
        auto mejorAsk = this->askMap.begin();
    
        // Spread positivo o nulo: el comprador está dispuesto a pagar al menos lo que pide el vendedor
        if(mejorBid->first >= mejorAsk->first)
        {
            Orden& ordenCompra = mejorBid->second.front();
            Orden& ordenVenta = mejorAsk->second.front();

            double energia = std::min(ordenCompra.kwh, ordenVenta.kwh);
            double precioClearing = (ordenCompra.precio + ordenVenta.precio) / 2.0;

            if(energia < umbralMinimoKwh)
            {
                mejorBid->second.pop();
                mejorAsk->second.pop();
                if(mejorBid->second.empty()) bidMap.erase(mejorBid);
                if(mejorAsk->second.empty()) askMap.erase(mejorAsk);
                continue;
            }

            // Si el vendedor es la batería comunitaria, descontar la energía entregada
            if(ordenVenta.idNodo == bateria.getId())
            {
                bateria.liberarEnergia(energia);
            }

            ordenCompra.kwh -= energia;
            ordenVenta.kwh -= energia;

            TransaccionEnergia transaccion = {
                ordenVenta.idNodo,
                ordenCompra.idNodo,
                energia,
                precioClearing,
                hora
            };
            
            this->transacciones.push_back(transaccion);
            this->transaccionesMercado++;

            transaccion.logTransaccion();

            // Desencolar órdenes agotadas o re-encolar remanentes significativos
            mejorBid->second.pop();
            if(ordenCompra.kwh > umbralMinimoKwh)
            {
                mejorBid->second.push(ordenCompra);
            }

            mejorAsk->second.pop();
            if(ordenVenta.kwh > umbralMinimoKwh)
            {
                mejorAsk->second.push(ordenVenta);
            }
                
            if(mejorBid->second.empty()) 
            {
                bidMap.erase(mejorBid);
            }
            if(mejorAsk->second.empty()) 
            {
                askMap.erase(mejorAsk);
            }
        }
        else
        {
            // Ya no existen precios compatibles entre compradores y vendedores
            break;
        }
    }

    // Liquidar excedentes no colocados hacia la batería comunitaria
    this->liquidarExcedentes(bateria, precioBaseHorario, hora);

    // Persistir transacciones atómicamente en base de datos
    cp.persistirTransacciones(this->transacciones);

    // Registrar resumen del tick
    this->logTick(hora);
}

void GridManager::liquidarExcedentes(
    NodoAlmacenamiento& bateria, double precioBaseHorario, int hora)
{
    /* 
    Todo vendedor prosumidor con energía remanente no vendida al finalizar 
    el matching transfiere su excedente a la batería al precio base horario.
    Si la orden remanente pertenece a la propia batería, no se auto-compra.
    */
    constexpr double umbralMinimoKwh = 0.001;

    while(!this->askMap.empty())
    {
        auto ask = this->askMap.begin();
        Orden& ordenVenta = ask->second.front();
        
        // Evitar que la batería se compre a sí misma su propia oferta no vendida
        if(ordenVenta.idNodo != bateria.getId() && ordenVenta.kwh > umbralMinimoKwh)
        {
            bateria.absorberEnergia(ordenVenta.kwh);

            TransaccionEnergia transaccionBateria
            {
                ordenVenta.idNodo,
                bateria.getId(),
                ordenVenta.kwh,
                precioBaseHorario,
                hora
            };

            this->transacciones.push_back(transaccionBateria);
            this->transaccionesExcedente++;

            std::cout << "\n[Excedente] Nodo " << ordenVenta.idNodo 
                      << " transfiere " << ordenVenta.kwh 
                      << " kWh a la batería al precio base de " << precioBaseHorario << ".\n";
        }

        ask->second.pop();
        if(ask->second.empty())
        {
            this->askMap.erase(ask);
        }
    }
}    

void GridManager::logTick(int hora) const
{
    std::cout
        << "\n=================> Resumen del Tick <=================\n"
        << " - Hora simulada: " << hora << ":00 hs\n"
        << " - Transacciones de mercado: " << this->transaccionesMercado << "\n"
        << " - Transacciones de excedentes hacia batería: " << this->transaccionesExcedente << "\n"
        << " - Total transacciones persistidas: " << this->transacciones.size() << "\n"
        << "======================================================\n"
        << std::endl;
}
