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

PerfilConsumo CapaDatos::stringAPerfil(const std::string& perfil)
{
    if(perfil == "Residencial")
        return PerfilConsumo::Residencial;

    if(perfil == "Comercial")
        return PerfilConsumo::Comercial;

    if(perfil == "Industrial")
        return PerfilConsumo::Industrial;

    throw std::runtime_error("Perfil de consumo inválido.");
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
            std::string perfilStr = fila.get<std::string>("perfil_consumo");
            PerfilConsumo perfil = this->stringAPerfil(perfilStr);

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
                    fila.get<int>("id_nodo"),
                    0,
                    fila.get<double>("saldo_cuenta")
                )
            );
        }
        else
        {
            nodos.push_back(
                std::make_unique<NodoAlmacenamiento>(
                fila.get<int>("id_nodo"),
                0,
                fila.get<double>("saldo_cuenta")
                )
            );
        }
    }    
    return nodos;
}

double CapaDatos::obtenerPrecioBase(int hora)
{
    //inicializo el contenedor del resultado de la consulta     
    double precio_base = 0.0;

    this->sql << "SELECT precio_base_kwh FROM config_tarifas WHERE hora=:hora",
        soci::use(hora), soci::into(precio_base);

    std::cout << "\n¡Consulta de precio base hecha con éxito!" << std::endl;

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

            this->actualizarSaldoYLecturas(
                t.idVendedor,
                t.kwh,
                t.precio,
                "venta",
                t.hora
            );
                       
            this->actualizarSaldoYLecturas(
                t.idComprador,
                t.kwh,
                t.precio,
                "compra",
                t.hora
            );
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
            :hora   
        )
    )", 
    soci::use(t.idVendedor, "vendedor"),
    soci::use(t.idComprador, "comprador"),
    soci::use(t.kwh, "kwh"),
    soci::use(t.precio, "precio"),
    soci::use(t.hora, "hora");   
}

void CapaDatos::actualizarSaldoYLecturas
    (int idNodo, double kwh, double precio, const std::string& tipo, int hora)
{
    this->sql <<
    R"(
        CALL actualizar_saldo_y_lecturas
        (
            :idNodo,
            :kwh,
            :precio,
            :tipo_operacion,
            :hora
        )
    )",
    soci::use(idNodo, "idNodo"),
    soci::use(kwh, "kwh"),
    soci::use(precio, "precio"),
    soci::use(tipo, "tipo_operacion"),
    soci::use(hora, "hora");
}
