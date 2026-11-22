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

using BidMap = std::map<double, std::queue<Orden>, std::greater<double>>;
using AskMap = std::map<double, std::queue<Orden>>; 

class GridManager
{
private:
    std::vector<TransaccionEnergia> transacciones;
    BidMap bidMap; 
    AskMap askMap; 

    // ID negativo autodecremental para las órdenes generadas por la batería
    int idOrdenBateria = 0;

    // Métricas por tick
    int transaccionesMercado = 0;
    int transaccionesExcedente = 0;

    void cargarLibroDeOrdenes(const std::vector<Orden>& ordenes);
    void inyectarBateriaEnMatching(NodoAlmacenamiento& bateria, double precioBaseHorario);    

public:
    int getIdOrdenBateria() const;
    void setIdOrdenBateria(int id);

    void procesarTick(
        NodoAlmacenamiento& bateria, 
        const std::vector<Orden>& ordenes, 
        int hora, 
        CapaDatos& cd
    );

    void liquidarExcedentes(
        NodoAlmacenamiento& bateria, 
        double precioBaseHorario, 
        int hora
    );

    void logTick(int hora) const;
};

#endif
