#include "LectorCSV.hpp"
#include <cctype>
#include <stdexcept>

std::string LectorCSV::trim(const std::string& str)
{
    auto inicio = str.find_first_not_of(" \t\r\n");
    if (inicio == std::string::npos) return "";
    auto fin = str.find_last_not_of(" \t\r\n");
    return str.substr(inicio, fin - inicio + 1);
}

bool LectorCSV::parsearEsCompra(const std::string& campo, int numeroLinea, const std::string& archivo)
{
    std::string s = trim(campo);
    for (char &c : s)
    {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }

    if (s == "compra") return true;
    if (s == "venta") return false;

    throw std::runtime_error(
        "[LectorCSV] Error en " + archivo + ":" + std::to_string(numeroLinea) + 
        " - Operación '" + campo + "' inválida. Debe ser 'compra' o 'venta'."
    );
}

Orden LectorCSV::parsearLinea(const std::string& linea, int numeroLinea, const std::string& archivo)
{
    std::stringstream ss(linea);
    std::vector<std::string> campos;
    std::string campo;

    while (std::getline(ss, campo, ','))
    {
        campos.push_back(trim(campo));
    }

    if (campos.size() != static_cast<size_t>(CANT_CAMPOS))
    {
        throw std::runtime_error(
            "[LectorCSV] Error en " + archivo + ":" + std::to_string(numeroLinea) + 
            " - Cantidad de campos incorrecta (esperados " + std::to_string(CANT_CAMPOS) + 
            ", obtenidos " + std::to_string(campos.size()) + "). Contenido: '" + linea + "'"
        );
    }

    int idOrden = 0;
    int idNodo = 0;
    double kwh = 0.0;
    double precio = 0.0;
    bool esCompra = false;

    try
    {
        idOrden = std::stoi(campos[0]);
        esCompra = parsearEsCompra(campos[1], numeroLinea, archivo);
        idNodo = std::stoi(campos[2]);
        kwh = std::stod(campos[3]);
        precio = std::stod(campos[4]);
    }
    catch (const std::invalid_argument&)
    {
        throw std::runtime_error(
            "[LectorCSV] Error en " + archivo + ":" + std::to_string(numeroLinea) + 
            " - Formato numérico inválido en línea: '" + linea + "'"
        );
    }
    catch (const std::out_of_range&)
    {
        throw std::runtime_error(
            "[LectorCSV] Error en " + archivo + ":" + std::to_string(numeroLinea) + 
            " - Valor numérico fuera de rango en línea: '" + linea + "'"
        );
    }

    // Validaciones semánticas del dominio
    if (idOrden <= 0)
    {
        throw std::runtime_error("[LectorCSV] " + archivo + ":" + std::to_string(numeroLinea) + " - idOrden debe ser mayor a 0.");
    }
    if (idNodo <= 0)
    {
        throw std::runtime_error("[LectorCSV] " + archivo + ":" + std::to_string(numeroLinea) + " - idNodo debe ser mayor a 0.");
    }
    if (kwh <= 0.0)
    {
        throw std::runtime_error("[LectorCSV] " + archivo + ":" + std::to_string(numeroLinea) + " - kwh debe ser estrictamente positivo (>0).");
    }
    if (precio < 0.0)
    {
        throw std::runtime_error("[LectorCSV] " + archivo + ":" + std::to_string(numeroLinea) + " - precio no puede ser negativo.");
    }

    return Orden{idOrden, esCompra, idNodo, kwh, precio};
}

std::vector<Orden> LectorCSV::leerOrdenes(const std::string& archivoCSV)
{
    std::ifstream archivo(archivoCSV);

    if(!archivo.is_open())
    {
        throw std::runtime_error("[LectorCSV] No se pudo abrir el archivo: " + archivoCSV);    
    }

    std::vector<Orden> ordenes;
    std::string linea;

    // Consumir cabecera si existe
    if (!std::getline(archivo, linea))
    {
        return ordenes; // Archivo vacío
    }

    int numeroLinea = 1;
    while(std::getline(archivo, linea))
    {
        numeroLinea++;
        std::string limpia = trim(linea);
        if (limpia.empty())
        {
            continue; // Ignorar líneas en blanco
        }

        Orden ord = parsearLinea(limpia, numeroLinea, archivoCSV);
        ordenes.push_back(ord);
    }

    return ordenes;
}
