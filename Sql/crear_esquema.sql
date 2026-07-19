DROP TRIGGER IF EXISTS trg_validar_saldo
ON transacciones;

DROP FUNCTION IF EXISTS validar_saldo_comprador();

DROP PROCEDURE IF EXISTS actualizar_saldo_y_lecturas(
    INTEGER,
    NUMERIC,
    NUMERIC,
    VARCHAR,
    CHAR
);

DROP TABLE IF EXISTS lecturas_historicas CASCADE;

DROP TABLE IF EXISTS transacciones CASCADE;

DROP TABLE IF EXISTS config_tarifas CASCADE;

DROP TABLE IF EXISTS nodos CASCADE;

CREATE TABLE IF NOT EXISTS nodos(
    id_nodo INTEGER PRIMARY KEY,

    tipo VARCHAR(20) NOT NULL
        CHECK (tipo IN ('Consumidor','Prosumidor','Bateria')),

    saldo_cuenta NUMERIC(15,2) NOT NULL DEFAULT 0
        CHECK (saldo_cuenta >= 0),

    balance_energia NUMERIC(10,2) NOT NULL DEFAULT 0,

    perfil_consumo VARCHAR(20)
);

CREATE TABLE IF NOT EXISTS lecturas_historicas(
    id_lectura SERIAL PRIMARY KEY,

    id_nodo INTEGER NOT NULL,

    hora CHAR(2)
        CHECK (hora BETWEEN '00' AND '23'),

    kwh NUMERIC(10,2) NOT NULL,

    precio_unitario NUMERIC(10,2) NOT NULL,

    tipo_operacion VARCHAR(10)
        CHECK (tipo_operacion IN ('compra','venta')),

    FOREIGN KEY(id_nodo)
        REFERENCES nodos(id_nodo)
);

CREATE TABLE IF NOT EXISTS transacciones(
    id_transaccion SERIAL PRIMARY KEY,

    id_vendedor INTEGER NOT NULL,

    id_comprador INTEGER NOT NULL,

    kwh NUMERIC(10,2) NOT NULL
        CHECK (kwh > 0),

    precio_unitario NUMERIC(10,2) NOT NULL
        CHECK (precio_unitario > 0),

    hora CHAR(2)
        CHECK (hora BETWEEN '00' AND '23'),

    FOREIGN KEY(id_vendedor)
        REFERENCES nodos(id_nodo),

    FOREIGN KEY(id_comprador)
        REFERENCES nodos(id_nodo)
);

CREATE TABLE IF NOT EXISTS config_tarifas(
    hora SMALLINT PRIMARY KEY
        CHECK (hora BETWEEN 0 AND 23),

    precio_base_kwh NUMERIC(10,2) NOT NULL
        CHECK(precio_base_kwh >= 0)
);

CREATE OR REPLACE FUNCTION validar_saldo_comprador()
RETURNS TRIGGER
AS $$
DECLARE
    v_saldo NUMERIC(15,2);
BEGIN

    --obtener saldo del comprador de la transacci´n
    SELECT saldo_cuenta
    INTO v_saldo
    FROM nodos
    WHERE id_nodo = NEW.id_comprador;

    --verificar si puede acaparar el precio total de la transacción
    IF v_saldo < NEW.kwh * NEW.precio_unitario THEN
        RAISE EXCEPTION
            '¡Saldo insuficiente para realizar la compra!';
    END IF;

    RETURN NEW;

END;
$$
LANGUAGE plpgsql;

-- trigger que invoca a la funcion de arriba
CREATE TRIGGER trg_validar_saldo
BEFORE INSERT ON transacciones
FOR EACH ROW
EXECUTE FUNCTION validar_saldo_comprador();

-- procedimiento almacenado que 
CREATE OR REPLACE PROCEDURE actualizar_saldo_y_lecturas(

    p_id_nodo INTEGER,
    p_kwh NUMERIC,
    p_precio NUMERIC,
    p_tipo_operacion VARCHAR(10),
    p_hora CHAR(2)

)
LANGUAGE plpgsql
AS $$
DECLARE

    v_importe NUMERIC;

BEGIN

    v_importe := p_kwh * p_precio;

    IF p_tipo_operacion = 'venta' THEN

        UPDATE nodos
        SET saldo_cuenta = saldo_cuenta + v_importe,
            balance_energia = balance_energia - p_kwh
        WHERE id_nodo = p_id_nodo;

    ELSE

        UPDATE nodos
        SET saldo_cuenta = saldo_cuenta - v_importe,
            balance_energia = balance_energia + p_kwh
        WHERE id_nodo = p_id_nodo;

    END IF;

END;
$$;

--INSERTAR DATOS EN LAS TABLAS CORRESPONDIENTES

INSERT INTO config_tarifas(hora, precio_base_kwh)
VALUES
(10,0),
(12,1.50),
(14,1.00),
(15,1.20),
(18,2.00);


INSERT INTO nodos
(id_nodo, tipo, saldo_cuenta, balance_energia, perfil_consumo)
VALUES
-- Batería comunitaria
(99, 'Bateria', 999999999.99, 0, NULL),

-- Caso 1
(1, 'Consumidor', 100.00, 0, 'Residencial'),
(2, 'Prosumidor', 50.00, 0, NULL),

-- Caso 2
(10, 'Consumidor', 500.00, 0, 'Industrial'),
(11, 'Consumidor', 200.00, 0, 'Comercial'),

(20, 'Prosumidor', 100.00, 0, NULL),
(21, 'Prosumidor', 80.00, 0, NULL),
(22, 'Prosumidor', 50.00, 0, NULL),

-- Caso 3
(30, 'Consumidor', 15.00, 0, 'Residencial'),
(40, 'Prosumidor', 60.00, 0, NULL),

-- Caso 4
(50, 'Prosumidor', 100.00, 0, NULL),
(51, 'Consumidor', 200.00, 0, 'Residencial');