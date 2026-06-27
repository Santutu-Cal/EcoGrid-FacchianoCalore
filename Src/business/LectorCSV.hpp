#ifndef LECTORCSV_HPP
#define LECTORCSV_HPP

#include "Orden.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#define CANT_COLUMNAS 5

class LectorCSV
{    
private:
    Orden parsearLinea(const std::string& linea);

public:
    std::vector<Orden> leerOrdenes(const std::string& archivoCSV);  
};

#endif