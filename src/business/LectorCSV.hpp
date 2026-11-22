#ifndef LECTORCSV_HPP
#define LECTORCSV_HPP

#include "Orden.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

constexpr int CANT_CAMPOS = 5;

class LectorCSV
{    
private:
    Orden parsearLinea(const std::string& linea, int numeroLinea, const std::string& archivo);
    static std::string trim(const std::string& str);
    static bool parsearEsCompra(const std::string& campo, int numeroLinea, const std::string& archivo);

public:
    std::vector<Orden> leerOrdenes(const std::string& archivoCSV);  
};

#endif