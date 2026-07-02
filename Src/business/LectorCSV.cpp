#ifndef LECTORCSV_CPP
#define LECTORCSV_CPP

#include "Simulador.hpp"

//función que parsea de string a bool
bool esCompraFunct(std::string string)
{
    //convierte cada carácter a minúscula usando un bucle range-based
    for(char &c : string) 
    {
        c = tolower(c);
    }
    return (string == "compra");
}

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
            if(i==2) esCompra = esCompraFunct(campo);
            if(i==3) idNodo = std::stoi(campo);
            if(i==4) kwh = std::stod(campo);
            if(i==5) precio = std::stod(campo);
        }
        
        Orden ord = {
            idOrden, 
            esCompra, 
            idNodo, 
            kwh, 
            precio
        };

        return ord;
}

/*
leerOrdenes:
1) Abrir el archivo.
2) Ignorar la cabecera.
3) Leer línea por línea (los datos importantes del archivo).
4) Convertir cada línea en un objeto Orden.
5) Guardar cada Orden en un vector.
6) Devolver el vector.
*/
std::vector<Orden> LectorCSV::leerOrdenes(const std::string& archivoCSV)
{
    std::ifstream archivo(archivoCSV);

    /*
    is_open() devolverá false en cualquiera de estas situaciones:
    - El archivo no existe.
    - El nombre del archivo está mal escrito.
    - La ruta es incorrecta.
    - El directorio no existe.
    - No tenés permisos para abrirlo.
    - Ocurrió algún error del sistema al intentar abrirlo.
    */
    if(!archivo.is_open())
    {
        std::cout << "Desde LectorCSV.cpp\n" << std::endl; 
        throw 
        std::runtime_error("No se pudo abrir el archivo: " + archivoCSV);    
    }

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