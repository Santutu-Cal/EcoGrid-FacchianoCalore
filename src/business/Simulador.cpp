#include "Simulador.hpp"
#include <filesystem>
#include <iostream>

int Simulador::extraerHoraDeArchivo(const std::string& rutaArchivo)
{
    std::filesystem::path p(rutaArchivo);
    std::string stem = p.stem().string(); // e.g. "ofertas_10"
    
    auto posGuion = stem.rfind('_');
    if (posGuion == std::string::npos || posGuion + 1 >= stem.size())
    {
        throw std::runtime_error(
            "[Simulador] No se pudo deducir la hora desde el nombre del archivo: " + rutaArchivo
        );
    }

    try
    {
        int hora = std::stoi(stem.substr(posGuion + 1));
        if (hora < 0 || hora > 23)
        {
            throw std::out_of_range("Hora fuera de rango (0-23)");
        }
        return hora;
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error(
            "[Simulador] Formato de hora inválido en nombre de archivo '" + rutaArchivo + "': " + e.what()
        );
    }
}

void Simulador::ejecutar()
{
    std::cout << "\n============================================\n"
              << "       INICIANDO SIMULADOR ECOGRID          \n"
              << "============================================\n";

    // 1. Conexión a la base de datos y recuperación de nodos
    try
    {
        this->cp.conectar();
        this->nodos = this->cp.obtenerNodos();
        std::cout << "[Simulador] Nodos cargados exitosamente desde la base de datos (" 
                  << this->nodos.size() << " nodos en total).\n";
    }
    catch(const std::exception& e)
    {
        std::cerr << "\n[Simulador] Error fatal de inicialización: " << e.what() << "\n"
                  << "Verifique que el servicio PostgreSQL esté activo y las credenciales en config.ini sean correctas.\n";
        throw;
    }

    // 2. Localizar nodo de almacenamiento (batería)
    this->grid.setIdOrdenBateria(0);

    NodoAlmacenamiento* b = nullptr;
    for(const auto& nodo : this->nodos)
    {
        b = dynamic_cast<NodoAlmacenamiento*>(nodo.get());
        if(b)
        {
            break;
        }    
    }

    if(!b)
    {
        throw std::runtime_error(
            "[Simulador] Error de topología: No se encontró ningún NodoAlmacenamiento (Batería) en la red."
        );
    }

    NodoAlmacenamiento& bateria = *b;
    std::cout << "[Simulador] Batería comunitaria identificada (ID: " << bateria.getId() 
              << ", Carga inicial: " << bateria.getCargaActual() << " kWh).\n";

    // 3. Procesar ticks secuencialmente
    for(const auto& archivoCSV : this->archivos)
    {
        int hora = extraerHoraDeArchivo(archivoCSV);
        std::cout << "\n>>> PROCESANDO TICK HORA " << hora << ":00 (" << archivoCSV << ") <<<\n";

        try
        {
            auto ordenes = this->lector.leerOrdenes(archivoCSV);
            std::cout << "[Simulador] Órdenes leídas para el tick: " << ordenes.size() << "\n";
            this->grid.procesarTick(bateria, ordenes, hora, this->cp);
        }
        catch(const std::exception& e)
        {
            std::cerr << "[Simulador] Error durante el procesamiento del tick (" << archivoCSV << "): " 
                      << e.what() << "\n";
            throw;
        }
    }    

    // 4. Cierre ordenado
    this->cp.desconectar();
    std::cout << "\n============================================\n"
              << "          SIMULACIÓN COMPLETADA CON ÉXITO   \n"
              << "============================================\n";
}
