#ifndef SIMULADOR_HPP
#define SIMULADOR_HPP

#include "GridManager.hpp"
#include "LectorCSV.hpp"

#include <filesystem>
namespace fs = std::filesystem;

#include <vector>

class Simulador
{
private:
    GridManager grid;
    LectorCSV lector;
    
    std::vector<fs::path> archivos;
    void encuentraArchivosCSV();

public:
    void ejecutar();    
};

void Simulador::encuentraArchivosCSV()
{
    fs::path carpeta = "Data";
    
    for(const auto& entrada : fs::directory_iterator(carpeta))
    {
        if(entrada.path().extension() == ".csv")
        {
            this->archivos.push_back(entrada.path());
        }        
    }

    std::sort(this->archivos.begin(), this->archivos.end());
}

void Simulador::ejecutar()
{
    LectorCSV lector;
}

#endif