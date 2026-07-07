#ifndef CAPADATOS_HPP
#define CAPADATOS_HPP

#include <string>
#include <memory>

#include <soci/soci.h> //include principal de SOCI
#include <soci/postgresql/soci-postgresql.h> //cargar backend de PostgreSQL

//probablemente no se use acá
#include "TransaccionEnergia.hpp"

#include "ConfigReader.hpp"

class CapaDatos
{
private:
    //contiene las credenciales de conexion (provenientes de config.ini)
    std::string conexion;
    ConfigReader cr;

    soci::session sql;

public:
    void persistirTransacciones();
    void obtenerPrecioBase();
    void cargarNodos();


    //prueba de bdd (no influye en el tema central del tp)
    //probar que la clase pueda mínimamente una conexion con la bdd
    void conectar();
    void desconectar(); //opcional
    void obtenerSesion();
    
    //
    void consulta();
    void update();
};

#endif