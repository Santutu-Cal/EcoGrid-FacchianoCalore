#ifndef CAPADATOS_HPP
#define CAPADATOS_HPP

#include <soci/soci.h> //include principal de SOCI
#include <soci/postgresql/soci-postgresql.h> //cargar backend de PostgreSQL

#include "ConfigReader.hpp"

#include "NodoRed.hpp"
#include "NodoConsumidor.hpp"
#include "NodoProsumidor.hpp"
#include "NodoAlmacenamiento.hpp"

#include "TransaccionEnergia.hpp"

#include <iostream>
#include <string>
#include <memory>
#include <vector>

class CapaDatos
{
private:
    //contiene las credenciales de conexion (provenientes de config.ini)
    std::string conexion;
    ConfigReader cr;

    //objeto que sostiene la conexion con la bdd
    soci::session sql;

    //ambas usadas en "persistirTransaccion"
    //insertar transaccion
    void insertarTransaccion(const TransaccionEnergia& t);

    //ejecutar proc almacenado de los nodos participantes de la transaccion
    void actualizarSaldoYLecturas(
        int idNodo, double kwh, double precio, const std::string& tipo);

public:

    //establecer conexion
    void conectar();

    //obtener precio base correspondiente
    double obtenerPrecioBase(std::string hora);

    //consultar nodos (los obtiene) de la bdd
    std::vector<std::unique_ptr<NodoRed>> obtenerNodos();

    //persistir transacciones hechas en un tick
    void CapaDatos::persistirTransacciones
        (std::vector<TransaccionEnergia>& transacciones);
    
    //desconectar bdd
    void desconectar();

    //-----------------------------------------------
    //prueba de bdd (no influye en la entrega del tp)
    void insert();
    void select();
    void update();
    void myDelete();
    void mostrarTabla();
    //------------------------------------------------    
};

#endif