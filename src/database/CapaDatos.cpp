#include "CapaDatos.hpp"
#include <algorithm>
#include <cctype>

void CapaDatos::conectar()
{
    try
    {
        //obtiene las credenciales del archivo config.ini
        this->conexion = this->cr.leerConfig("src/database/config.ini");

        //abre la conexion a la bdd
        this->sql.open(soci::postgresql, this->conexion);

        std::cout << "\n¡Conexion exitosa a la bdd!" << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << "\n[CapaDatos::conectar] Error de conexion: " 
            << e.what() << std::endl;
        throw; // Relanza la excepción para que el llamador no opere sobre conexión nula
    }
}

bool CapaDatos::estaConectado() const
{
    return const_cast<soci::session&>(this->sql).is_connected();
}

void CapaDatos::desconectar()
{
    if(this->estaConectado())
    {
        this->sql.close();
        std::cout << "\nDesconexión exitosa de la bdd." << std::endl;
    }
}

PerfilConsumo CapaDatos::stringAPerfil(const std::string& perfil)
{
    std::string p = perfil;
    std::transform(p.begin(), p.end(), p.begin(), [](unsigned char c){ return std::tolower(c); });

    if(p == "residencial")
        return PerfilConsumo::Residencial;

    if(p == "comercial")
        return PerfilConsumo::Comercial;

    if(p == "industrial")
        return PerfilConsumo::Industrial;

    throw std::runtime_error("Perfil de consumo inválido: " + perfil);
}

std::vector<std::unique_ptr<NodoRed>> CapaDatos::obtenerNodos()
{
    if(!this->estaConectado())
    {
        throw std::runtime_error("[CapaDatos::obtenerNodos] No hay conexión activa a la base de datos.");
    }

    std::vector<std::unique_ptr<NodoRed>> nodos;

    //contiene todos los nodos que están en la tabla "nodos" en la bdd
    soci::rowset<soci::row> filas = 
        (this->sql.prepare << "SELECT id_nodo, tipo, saldo_cuenta, balance_energia, perfil_consumo FROM nodos");

    for(soci::row& fila : filas)
    {
        std::string tipo = fila.get<std::string>("tipo");
        std::string tipoLower = tipo;
        std::transform(tipoLower.begin(), tipoLower.end(), tipoLower.begin(), [](unsigned char c){ return std::tolower(c); });

        int idNodo = fila.get<int>("id_nodo");
        double saldo = fila.get<double>("saldo_cuenta");
        double balance = fila.get<double>("balance_energia");

        if(tipoLower == "consumidor")
        {
            std::string perfilStr = fila.get<std::string>("perfil_consumo");
            PerfilConsumo perfil = this->stringAPerfil(perfilStr);

            nodos.push_back(
                std::make_unique<NodoConsumidor>(
                    idNodo,
                    balance,
                    saldo,
                    perfil
                )
            );
        }
        else if(tipoLower == "prosumidor")
        {
            nodos.push_back(
                std::make_unique<NodoProsumidor>(
                    idNodo,
                    balance,
                    saldo
                )
            );
        }
        else if(tipoLower == "bateria")
        {
            nodos.push_back(
                std::make_unique<NodoAlmacenamiento>(
                    idNodo,
                    balance,
                    saldo
                )
            );
        }
        else
        {
            throw std::runtime_error("[CapaDatos::obtenerNodos] Tipo de nodo desconocido en BD: " + tipo);
        }
    }    
    return nodos;
}

double CapaDatos::obtenerPrecioBase(int hora)
{
    if(!this->estaConectado())
    {
        throw std::runtime_error("[CapaDatos::obtenerPrecioBase] No hay conexión activa a la base de datos.");
    }

    if(hora < 0 || hora > 23)
    {
        throw std::out_of_range("[CapaDatos::obtenerPrecioBase] Hora fuera de rango (0-23): " + std::to_string(hora));
    }

    //inicializo el contenedor del resultado de la consulta     
    double precio_base = 0.0;

    this->sql << "SELECT precio_base_kwh FROM config_tarifas WHERE hora=:hora",
        soci::use(hora), soci::into(precio_base);

    std::cout << "\n¡Consulta de precio base hecha con éxito para la hora " << hora << ":00!" << std::endl;

    return precio_base;
}

void CapaDatos::persistirTransacciones
    (std::vector<TransaccionEnergia>& transacciones)
{
    if(!this->estaConectado())
    {
        throw std::runtime_error("[CapaDatos::persistirTransacciones] No hay conexión activa a la base de datos.");
    }

    if(transacciones.empty())
    {
        std::cout << "\n[CapaDatos::persistirTransacciones] No hay transacciones para persistir en este tick.\n";
        return;
    }

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
        std::cerr << "\n[CapaDatos::persistirTransacciones] Error transaccional (ejecutando ROLLBACK): " 
                  << e.what() << '\n';
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
