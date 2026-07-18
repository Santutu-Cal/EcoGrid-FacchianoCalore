CREATE TABLE IF NOT EXISTS NODOS(
    id_nodo INTEGER PRIMARY KEY,

    tipo VARCHAR(20) CHECK (IN ('Consumidor', 'Prosumidor', 'Bateria')),
    saldo_cuenta DECIMAL(15,2) DEFAULT 0 CHECK (saldo_cuenta >= 0) ,
    perfil_consumo VARCHAR(20)
);

CREATE TABLE IF NOT EXISTS LECTURAS_HISTORICAS(
    id_lectura SERIAL PRIMARY KEY,

    id_nodo INTEGER NOT NULL,
    hora CHAR(2) NOT NULL CHECK (hora BETWEEN '00' and '23'),
    kwh NUMERIC(10,2) NOT NULL,
    precio_unitario(10,2) NOT NULL,
    tipo VARCHAR(20) CHECK (IN ('Consumidor', 'Prosumidor')),

    FOREIGN KEY(id_nodo) REFERENCES nodos(id_nodo)
);

CREATE TABLE IF NOT EXISTS TRANSACCIONES(
    id_transaccion SERIAL PRIMARY KEY,

    id_vendedor INTEGER NOT NULL,
    id_comprador INTEGER NOT NULL,
    kwh DECIMAL(10,2) NOT NULL CHECK (kwh > 0)
    precio_unitario DECIMAL(10,2) NOT NULL CHECK (precio_unitario > 0),
    hora CHAR(2) NOT NULL CHECK (hora BETWEEN '00' and '23'),

    FOREIGN KEY (id_vendedor) REFERENCES nodo(id_nodo)
    FOREIGN KEY (id_comprador) REFERENCES nodo(id_nodo)
);

CREATE TABLE IF NOT EXISTS CONFIG_TARIFAS(
    hora INTEGER PRIMARY KEY CHECK (hora BETWEEN '00' and '23'),

    precio_base_kwh DECIMAL(10,2) NOT NULL
);

-- crear función que hace la validación
CREATE OR REPLACE FUNCTION validar_saldo_comprador()
RETURNS TRIGGER AS $$
DECLARE
    v_saldo_comprador DECIMAL;
BEGIN
    -- Lógica de validación
    SELECT saldo_cuenta INTO v_saldo_comprador
    FROM NODOS WHERE id_nodo = NEW.id_comprador;
    
    IF v_saldo_comprador < (NEW.kwh * NEW.precio_unitario) THEN
        RAISE EXCEPTION '¡Saldo insuficiente para realizar la compra!';
    END IF;
    
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

-- crear trigger que invoca a la función de validación
CREATE TRIGGER trg_validar_saldo
BEFORE INSERT ON TRANSACCIONES
FOR EACH ROW
EXECUTE FUNCTION validar_saldo_comprador();

-- crear funcion que actualiza la tabla "saldo_lecturas"
CREATE OR REPLACE PROCEDURE actualizar_saldo_y_lecturas(
    p_id_nodo INTEGER,
    p_kwh NUMERIC,
    p_precio NUMERIC,
    p_tipo_operacion VARCHAR(10),
    p_hora CHAR(2)
)

