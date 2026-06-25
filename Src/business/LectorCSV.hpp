#ifndef LECTORCSV_HPP
#define LECTORCSV_HPP

#include "Orden.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

#define CANT_COLUMNAS 5

//función que parsea de string a bool
bool stob(std::string string)
{
    //convierte cada carácter a minúscula usando un bucle range-based
    for(char &c : string) 
    {
        c = tolower(c);
    }
    return (string == "compra" || string == "1");
}

class LectorCSV
{    
private:
    Orden parsearLinea(const std::string& linea);

public:
    std::vector<Orden> leerOrdenes(const fs::path& pathArchivo);  
};

/*
Toma una línea (fila) extraída del archivo y la procesa, devolviéndola en forma-
-to de Orden.
*/
Orden LectorCSV::parsearLinea(const std::string& linea)
{
        /*
        std::stringstream permite tratar un std::string como si fuera un 
        archivo o como si fuera un std::cin (flujo de entrada). O sea, sirve
        para tratar a la linea (fila) que tomó del archivo como si fuera un 
        flujo de entrada al programa.
        Todos son streams (flujos). La diferencia es la fuente de datos:
        cin → teclado.
        ifstream → archivo.
        stringstream → un string que ya está en memoria.
        */ 
        std::stringstream ss(linea);
        
        //campos a rellenar
        int idOrden, idNodo;
        double kwh, precio;
        bool esCompra;

        for(int i=1; i<=CANT_COLUMNAS; i++)
        {
            std::string campo;
            std::getline(ss, campo, ',');

            if(i==1) idOrden = std::stoi(campo);
            if(i==2) esCompra = stob(campo);
            if(i==3) idNodo = std::stoi(campo);
            if(i==4) precio = std::stod(campo);
            if(i==5) kwh = std::stod(campo);
        }
        
        Orden ord = {
            idOrden, 
            esCompra, 
            idNodo, 
            precio, 
            kwh
        };

        return ord;
}

/*
leerOrdenes:
Abrir el archivo.
Ignorar la cabecera.
Leer línea por línea (los datos importantes del archivo).
Convertir cada línea en un objeto Orden.
Guardar cada Orden en un vector.
Devolver el vector.
*/
std::vector<Orden> LectorCSV::leerOrdenes(const fs::path& pathArchivo)
{
    std::ifstream archivo(pathArchivo);

    if(!archivo.is_open())
        throw std::runtime_error("\n¡No se pudo abrir el archivo!");    

    //vector que contendrá todas las ordenes a procesar
    std::vector<Orden> ordenes;

    std::string linea;
    //consumir primer línea (la que tiene datos cabecera de las columnas)
    std::getline(archivo, linea);

    //siempre que getline detecte un '\n' pasara a la siguiente fila o EOF. 
    while(std::getline(archivo, linea))
    {
        Orden ord = parsearLinea(linea);
        ordenes.push_back(ord);
    }    

    return ordenes;
}

#endif