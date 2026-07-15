#ifndef CAPADATOS_HPP
#define CAPADATOS_HPP

#include <string>
#include <memory>
#include <vector>

#include <soci/soci.h> //include principal de SOCI
#include <soci/postgresql/soci-postgresql.h> //cargar backend de PostgreSQL

#include "NodoRed.hpp"
#include "NodoConsumidor.hpp"
#include "NodoProsumidor.hpp"

#include "TransaccionEnergia.hpp"

#include "ConfigReader.hpp"

class CapaDatos
{
private:
    //contiene las credenciales de conexion (provenientes de config.ini)
    std::string conexion;
    ConfigReader cr;

    //objeto que sostiene la conexion con la bdd
    soci::session sql;

public:
    //persistir transacciones hechas en un tick
    void CapaDatos::persistirTransacciones
        (const std::vector<TransaccionEnergia>& transacciones);
    
    //consultar nodos (los obtiene) de la bdd
    std::vector<std::unique_ptr<NodoRed>> obtenerNodos();
    
    //obtener precio base correspondiente
    double obtenerPrecioBase(const std::string hora) const;

    //establecer conexion
    void conectar();

    //cancelar conexion
    void desconectar();
    
    //informacion de la conexion
    void obtenerSesion();

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