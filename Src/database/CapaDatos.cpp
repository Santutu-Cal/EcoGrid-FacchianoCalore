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


//-----------------------------------------------------
//acá empiezan las funciones que van en el TP realmente
void CapaDatos::conectar()
{
    try
    {
        //obtiene las credenciales del archivo config.ini
        this->conexion = this->cr.leerConfig("Src/database/config.ini");

        //abre la conexion a la bdd
        this->sql.open( soci::postgresql, this->conexion);

        std::cout << "\n¡Conexion exitosa a la bdd!" << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << "\nDesde CapaDatos::conectar\n *Error de conexion: " 
            << e.what() << std::endl;
    }
}

std::vector<std::unique_ptr<NodoRed>> CapaDatos::obtenerNodos()
{
    std::vector<std::unique_ptr<NodoRed>> nodos;

    //contiene todos los nodos que están en la tabla "nodos" en la bdd
    soci::rowset<soci::row> filas = 
        (this->sql.prepare << "SELECT * FROM nodos");

    for(soci::row& fila : filas)
    {
        std::string tipo = fila.get<std::string>("tipo");
        if(tipo=="consumidor")
        {
            nodos.push_back(
                std::make_unique<NodoConsumidor>(
                    fila.get<int>("id"),
                    0,
                    fila.get<double>("saldo_cuenta"),
                    perfil
                )
            );
        }
        else if(tipo=="prosumidor")
        {
            nodos.push_back(
                std::make_unique<NodoProsumidor>(
                    fila.get<int>("id"),
                    0,
                    saldoCuenta.fila.get<double>("saldo_cuenta")
                )
            );
        }
        else
        {
            nodos.push_back(
                std::make_unique<NodoAlmacenamiento>(
                fila.get<int>("id"),
                0,
                fila.get<double>("saldo_cuenta")
                )
            );
        }
    }    
    return nodos;
}

double obtenerPrecioBase(const std::string hora) const
{
    //inicializo el contenedor del resultado de la consulta     
    double precio_base = 0.0;
    this->sql << "SELECT precio_base FROM config_tarifas WHERE hora= :hora",
        soci::use(hora), soci::into(precio_base);
    std::cout << "\n¡Consulta de precio base hecha con éxito!";

    return precio_base;
}

void CapaDatos::persistirTransacciones
    (std::vector<TransaccionEnergia>& transacciones)
{
    soci::transaction tr(this->sql);

    for(const auto &t : transacciones)
    {
        sql << "INSERT INTO transacciones(id_vendedor, id_comprador, kwh, precio_unitario, hora) VALUES (:vendedor, :comprador, :kwh, :precio)", soci::use(t.idVendedor, t.idComprador, 
            t.kwh, t.precio);


    }
}

