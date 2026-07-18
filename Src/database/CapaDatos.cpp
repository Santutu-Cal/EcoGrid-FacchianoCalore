#include "CapaDatos.hpp"

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
                    fila.get<double>("saldo_cuenta")
                )
            );
        }
        else if(tipo=="prosumidor")
        {
            nodos.push_back(
                std::make_unique<NodoProsumidor>(
                    fila.get<int>("id"),
                    0,
                    fila.get<double>("saldo_cuenta")
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

double CapaDatos::obtenerPrecioBase(std::string hora)
{
    //inicializo el contenedor del resultado de la consulta     
    double precio_base = 0.0;

    this->sql << "SELECT precio_base_kwh FROM config_tarifas WHERE hora= :hora",
        soci::use(hora), soci::into(precio_base);

    std::cout << "\n¡Consulta de precio base hecha con éxito!";

    return precio_base;
}

void CapaDatos::persistirTransacciones
    (std::vector<TransaccionEnergia>& transacciones)
{
    /*
    iniciar (BEGIN) la transaccion, los insert y procedimientos que se hagan
    quedarán en pendiente hasta que se haga un "tr.commit" (COMMIT). 
    */
    soci::transaction tr(this->sql);

    try
    {
        for(const auto &t : transacciones)
        {
            this->insertarTransaccion(t);

            /*  
            Nota: La batería no genera lecturas históricas. Sólo se registran 
            los nodos consumidores y vendedores.
            */
            if(t.idVendedor != ID_BATERIA){
                this->actualizarSaldoYLecturas(
                    t.idVendedor,
                    t.kwh,
                    t.precio,
                    "venta"
                );
            }
            
            if(t.idComprador != ID_BATERIA){                
                this->actualizarSaldoYLecturas(
                    t.idComprador,
                    t.kwh,
                    t.precio,
                    "compra"
                );
            }
        }

        //commitear si hasta este punto no hubo ninguna excepción que manejar
        tr.commit();
    }
    catch(const std::exception &e)
    {
        /*
        Nota: El ROLLBACK es hecho automáticamente por el destructor de objeto
        "tr" declarado al principio de este método gracias a que sigue el prin -
        - cipio de RAII
        */

        std::cerr << e.what() << '\n';
        throw;
    }    
}

void CapaDatos::insertarTransaccion(const TransaccionEnergia& t)
{
    /*
    esa R que se ve al principio de la insert es una forma que tiene de 
    c++ para poder meter "enters" en el código sin producir conflictos 
    sintácticos"
    */   
    this->sql << 
    R"(
        INSERT INTO transacciones
        (
            id_vendedor, 
            id_comprador, 
            kwh, 
            precio_unitario, 
            hora
        ) 
        VALUES 
        (
            :vendedor, 
            :comprador, 
            :kwh, 
            :precio, 
            1:hora   
        )
    )", 
    soci::use(t.idVendedor);
    soci::use(t.idComprador);
    soci::use(t.kwh);
    soci::use(t.precio);
    soci::use(t.hora);   
}

void CapaDatos::actualizarSaldoYLecturas(
    int idNodo, double kwh, double precio, const std::string& tipo)
{
    this->sql <<
    R"(
        CALL actualizar_saldo_y_lecturas
        (
            :idNodo,
            :kwh,
            :precio,
            :tipo
        )
    )",
    soci::use(idNodo);
    soci::use(kwh);
    soci::use(precio);
    soci::use(tipo);
}
