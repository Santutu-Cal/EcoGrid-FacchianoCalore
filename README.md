# EcoGrid: Simulador de Micro-Redes Energéticas Inteligentes

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg?style=flat-square&logo=c%2B%2B)
![CMake](https://img.shields.io/badge/CMake-3.16%2B-064F8C.svg?style=flat-square&logo=cmake)
![PostgreSQL](https://img.shields.io/badge/PostgreSQL-16-336791.svg?style=flat-square&logo=postgresql)
![SOCI](https://img.shields.io/badge/SOCI-Database%20Abstraction-red.svg?style=flat-square)
![Architecture](https://img.shields.io/badge/Architecture-Layered%20%2F%20POO-brightgreen.svg?style=flat-square)

Sistema simulador de **micro-redes eléctricas distribuidas y mercados descentralizados peer-to-peer (P2P)** desarrollado en **C++17**, con persistencia transaccional sobre **PostgreSQL** mediante **SOCI** y lógica de negocio distribuida en servidor (Stored Procedures y Triggers).

---

## 📌 Contexto y Problema que Resuelve

En el paradigma energético tradicional, las redes son unidireccionales: grandes centrales generan energía y los consumidores finales la demandan pasivamente. La transición hacia ciudades inteligentes y energías renovables introduce una transformación radical: los ciudadanos instalan generación distribuida (paneles fotovoltaicos, microturbinas eólicas) y se convierten en **prosumidores** (productores + consumidores), capaces de comerciar sus excedentes directamente con sus vecinos en una micro-red local.

### El Desafío
Este escenario plantea dos retos críticos:
1. **Algorítmico y de Mercado:** ¿Cómo emparejar de forma justa, en tiempo real y con eficiencia de tiempo/precio la oferta variable de múltiples prosumidores con la demanda fluctuante de múltiples consumidores sin intermediarios centralizados que cobren comisiones abusivas?
2. **Estabilidad y Transaccionalidad:** La energía eléctrica no se puede desperdiciar si no hay comprador inmediato en el mercado; además, las transacciones financieras y lecturas de consumo deben cumplir con garantías de consistencia **ACID** para evitar saldos negativos o doble gasto de energía.

### La Solución de EcoGrid
EcoGrid modela un ecosistema de micro-red que opera en **ticks horarios** discretos:
* Implementa un motor de **Subasta Doble Continua (*Continuous Double Auction*)** en memoria RAM con ordenamiento estricto por prioridad precio-tiempo ($O(\log N)$ para inserción y $O(1)$ para despacho).
* Integra un **Nodo de Almacenamiento Comunitario (Batería)** que actúa como estabilizador: absorbe excedentes no vendidos al final de cada ciclo (*Buyer of Last Resort*) y los reinyecta al mercado en el siguiente tick a precio de referencia.
* Ejecuta la persistencia y liquidación en un bloque atómico en **PostgreSQL**, donde triggers validan la solvencia del comprador antes de consolidar la operación y procedimientos almacenados auditan consumos históricos.

---

## 🏗️ Arquitectura del Sistema

El software está diseñado bajo una **Arquitectura en Capas (*Layered Architecture*)** desacoplada, aplicando principios SOLID y patrones de diseño orientados a objetos:

```
┌─────────────────────────────────────────────────────────────┐
│                      Simulador (Main)                       │
│    Orquestador del ciclo de vida y secuencias de ticks      │
└──────────────────────────────┬──────────────────────────────┘
                               │
       ┌───────────────────────┴───────────────────────┐
       ▼                                               ▼
┌──────────────────────────────┐        ┌──────────────────────────────┐
│     GridManager (Negocio)    │        │       LectorCSV (I/O)        │
│   • Libro de Órdenes (RAM)   │        │   • Deserializador seguro    │
│   • Subasta Doble Continua   │        │   • Sanitización y validación│
│   • Liquidación de Batería   │        └──────────────────────────────┘
└──────────────┬───────────────┘
               │ opera sobre
               ▼
┌─────────────────────────────────────────────────────────────┐
│                       Modelo de Dominio                     │
│    NodoRed (Abstracto) ──┬── NodoConsumidor (Polimórfico)   │
│                          ├── NodoProsumidor (Polimórfico)   │
│                          └── NodoAlmacenamiento (Batería)   │
│    Entidades DTO: Orden, TransaccionEnergia                 │
└──────────────────────────────┬──────────────────────────────┘
                               │
                               ▼
┌─────────────────────────────────────────────────────────────┐
│                   CapaDatos (Persistencia)                  │
│    • Abstracción con SOCI C++                               │
│    • Transaccionalidad RAII (soci::transaction)             │
│    • ConfigReader (Sanitizador INI)                         │
└──────────────────────────────┬──────────────────────────────┘
                               │ TCP/IP
                               ▼
┌─────────────────────────────────────────────────────────────┐
│                    PostgreSQL Database                      │
│    • Tablas: nodos, transacciones, config_tarifas           │
│    • Trigger: trg_validar_saldo (Prevención de sobregiro)   │
│    • Procedure: actualizar_saldo_y_lecturas                 │
│    • Auditoría: lecturas_historicas                         │
└─────────────────────────────────────────────────────────────┘
```

---

## ⚙️ Características Técnicas Destacadas

* **Gestión de Memoria y RAII Estricto:** Uso exclusivo de punteros inteligentes (`std::unique_ptr`) para la propiedad polimórfica de los nodos. Cero fugas de memoria (`valgrind clean`).
* **Estructuras de Datos Eficientes:**
  * Libro de compras (*Bids*): `std::map<double, std::queue<Orden>, std::greater<double>>` ordenado descendente por precio.
  * Libro de ventas (*Asks*): `std::map<double, std::queue<Orden>>` ordenado ascendente por precio.
  * FIFO interno por nivel de precio mediante `std::queue<Orden>`.
* **Transacciones Atómicas con Rollback Automático:** La capa de persistencia encapsula el lote completo del tick en un `soci::transaction`. Si una transacción viola una restricción de negocio en PostgreSQL, el destructor de la transacción ejecuta un `ROLLBACK` automático por RAII sin corromper el estado de la base de datos.
* **Seguridad en Tiempo de Ejecución:** Validaciones en parsing de CSV (detección de valores negativos, formatos no numéricos y rutas de archivo relativas o absolutas dinámicas mediante `std::filesystem`).
* **Compilación Rigurosa:** Código C++17 limpio configurado para compilar con `-Wall -Wextra -pedantic` sin advertencias.

---

## 📂 Estructura del Repositorio

```text
EcoGrid/
├── CMakeLists.txt              # Configuración moderna de build CMake
├── README.md                   # Documentación principal de presentación
├── datos/                      # Datasets de órdenes horarias (CSV)
│   ├── ofertas_10.csv
│   ├── ofertas_12.csv
│   ├── ofertas_14.csv
│   ├── ofertas_15.csv
│   └── ofertas_18.csv
├── docs/                       # Documentación técnica profunda y diagramas
│   ├── patrones_y_diseno.md    # Deep-dive de Patrones de Diseño y POO
│   ├── algoritmo_subasta_y_transaccionalidad.md # Mecánica de matching y ACID
│   ├── diagrama_uml.png        # Diagrama de Clases UML
│   ├── diagrama_der.png        # Diagrama Entidad-Relación de base de datos
│   └── consigna_tp_ecogrid.pdf # Especificación formal del trabajo integrador
├── sql/
│   └── schema.sql              # DDL, DML inicial, triggers y stored procedures
└── src/
    ├── main.cpp                # Punto de entrada de la aplicación
    ├── business/               # Capa de Lógica de Negocio y Simulación
    │   ├── GridManager.cpp / .hpp
    │   ├── LectorCSV.cpp / .hpp
    │   └── Simulador.cpp / .hpp
    ├── database/               # Capa de Acceso a Datos y Persistencia
    │   ├── CapaDatos.cpp / .hpp
    │   ├── ConfigReader.cpp / .hpp
    │   └── config.ini          # Parámetros de conexión a PostgreSQL
    └── model/                  # Dominio, Entidades y Jerarquía de Clases
        ├── NodoRed.cpp / .hpp
        ├── NodoConsumidor.cpp / .hpp
        ├── NodoProsumidor.cpp / .hpp
        ├── NodoAlmacenamiento.cpp / .hpp
        ├── TransaccionEnergia.cpp / .hpp
        └── Orden.hpp
```

---

## 🚀 Guía de Inicio Rápido (Quickstart)

### Prerrequisitos
En sistemas basados en Debian/Ubuntu:
```bash
sudo apt update
sudo apt install -y build-essential cmake libpq-dev libsoci-dev postgresql postgresql-contrib
```

### 1. Configurar la Base de Datos
Inicia el servicio de PostgreSQL y crea el esquema:
```bash
# Iniciar PostgreSQL
sudo systemctl start postgresql

# Crear base de datos y usuario
sudo -u postgres psql -c "CREATE USER ecogrid_user WITH PASSWORD 'sancal';"
sudo -u postgres psql -c "CREATE DATABASE ecogrid OWNER ecogrid_user;"
sudo -u postgres psql -c "GRANT ALL PRIVILEGES ON DATABASE ecogrid TO ecogrid_user;"

# Ejecutar el script DDL con tablas, triggers, procedimientos y datos iniciales
psql -h localhost -U ecogrid_user -d ecogrid -f sql/schema.sql
```

Verifica o ajusta las credenciales en `src/database/config.ini`:
```ini
host=localhost
port=5432
dbname=ecogrid
user=ecogrid_user
password=sancal
```

### 2. Compilar el Proyecto
```bash
cd EcoGrid
cmake -B build -S .
cmake --build build
```

### 3. Ejecutar la Simulación
```bash
./build/EcoGrid
```

---

## 📚 Documentación Técnica Detallada (Deep Dives)

Para conocer en profundidad las decisiones de ingeniería, modelos matemáticos y fundamentos de diseño:

1. 📖 **[Patrones de Diseño y Arquitectura de Software](docs/patrones_y_diseno.md):**
   * Arquitectura en Capas y Aislamiento de Dominio.
   * Gestión de Recursos y Ciclo de Vida mediante RAII.
   * Jerarquía Polimórfica y Principio de Sustitución de Liskov (LSP).
   * Data Transfer Objects (DTO) e Invariantes de Dominio.
2. ⚡ **[Algoritmo de Subasta Doble Continua y Modelo Transaccional](docs/algoritmo_subasta_y_transaccionalidad.md):**
   * Funcionamiento detallado del libro de órdenes (*Order Book*).
   * Complejidades algorítmicas ($O(\log N)$ vs $O(1)$) y regla de precio medio (*clearing price*).
   * Modelo físico de la batería comunitaria (estabilizador y *Buyer of Last Resort*).
   * Atomicidad transaccional ACID con SOCI, triggers defensivos y procedimientos de auditoría.
