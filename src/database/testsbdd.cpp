#include "CapaDatos.hpp"

//funciones de prueba

//into trae datos SQL a C++
void CapaDatos::insert()
{
    std::string n = "Hector";

    this->sql << "INSERT INTO prueba(nombre) VALUES(:n)", soci::use(n);

    std::cout << "¡INSERT hecho con éxito!" << std::endl;
}


void CapaDatos::select()
{
    //recibe el resultado de la consulta
    int resultado;

    //into manda datos al SQL
    this->sql << "SELECT COUNT(nombre) FROM prueba", soci::into(resultado);

    std::cout << "¡SELECT hecho con éxito!\n";
    std::cout << "> La cantidad de nombres es: " << resultado << std::endl;
}

void CapaDatos::update()
{
    int id = 2;
    std::string nombre = "Morfeo";

    this->sql << "UPDATE prueba SET nombre=:nombre WHERE id=:id",
        soci::use(nombre),
        soci::use(id);

    std::cout << "¡UPDATE hecho con éxito!" << std::endl;   
}

void CapaDatos::myDelete()
{
    int id = 4;

    this->sql << "DELETE FROM prueba WHERE id=:id",
        soci::use(id);

    std::cout << "¡DELETE hecho con éxito!" << std::endl;     
}

void CapaDatos::mostrarTabla()
{
    soci::rowset<soci::row> filas = 
        (this->sql.prepare << "SELECT id, nombre FROM prueba ORDER BY id");

    std::cout << "--------------------------\n";
    
    for(soci::row& fila : filas)
    {
        int id = fila.get<int>("id");
        std::string nombre = fila.get<std::string>("nombre");

        std::cout << "ID: " << id
                  << " | Nombre: " << nombre
                  << '\n';
    }

    std::cout << "--------------------------\n";
}