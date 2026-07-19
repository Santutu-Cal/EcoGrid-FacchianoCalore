# define la version mínima de CMake a utilizar
cmake_minimum_required(VERSION 3.16)

# define el nombre del proyecto
project(EcoGrid)

# define el estandar de c++ a utilizar
set(CMAKE_CXX_STANDARD 17)

# obliga a usar C++17 y evita que el compilador "baje" a una versión - 
# - anterior si no encuentra soporte.
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# encuentra las dependencias para la bdd
find_package(PostgreSQL REQUIRED)

# encuentran las librerías para linkear los ejecutables sin errores

find_path(SOCI_INCLUDE_DIR
    NAMES soci/soci.h
)

find_library(SOCI_CORE_LIBRARY
    NAMES soci_core
)

find_library(SOCI_POSTGRESQL_LIBRARY
    NAMES soci_postgresql
)

# crea el ejetuable nombrando cada uno de los archivos ".cpp" del proyecto
add_executable(EcoGrid

    Src/main.cpp

    Src/business/Simulador.cpp
    Src/business/GridManager.cpp
    Src/business/LectorCSV.cpp

    Src/model/NodoRed.cpp
    Src/model/NodoConsumidor.cpp
    Src/model/NodoProsumidor.cpp
    Src/model/NodoAlmacenamiento.cpp
    Src/model/TransaccionEnergia.cpp

    Src/database/CapaDatos.cpp
    Src/database/ConfigReader.cpp
)

# ubicacion de los ".hpps" que deberá encontrar para compilar
target_include_directories(EcoGrid PRIVATE
    Src/business
    Src/model
    Src/database
    ${SOCI_INCLUDE_DIR}
    ${PostgreSQL_INCLUDE_DIRS}
)


# agrega warnings a la compilacion

# -Wall: advertencias, más comunes y útiles para errores típicos, como varia- 
# -bles sin usar, funciones obsoletas o pérdida de tipos.

# -Wextra: Activa advertencias adicionales que no están cubiertas por -Wall.
# Se enfoca en casos más específicos donde la lógica puede fallar, como 
# parámetros de funciones no utilizados, comparaciones con signos distintos 
# o problemas con estructuras.

# -pedantic: Exige que el código cumpla estrictamente con el estándar ISO C 
# o C++. Emite advertencias cuando utilizas extensiones del compilador (que  
# GCC permite por defecto) para asegurar que tu código sea 100% portable a 
# otros compiladores.  
target_compile_options(EcoGrid PRIVATE
    -Wall
    -Wextra
    -pedantic
)

# enlazar (link) las librerías
target_link_libraries(EcoGrid
    PRIVATE
        ${SOCI_CORE_LIBRARY}
        ${SOCI_POSTGRESQL_LIBRARY}
        ${PostgreSQL_LIBRARIES}
)
