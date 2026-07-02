# define la version mínima de CMake a utilizar
cmake_minimum_required(VERSION 3.16)

# define el nombre del proyecto
project(EcoGrid)

# define el estandar de c++ a utilizar
set(CMAKE_CXX_STANDARD 17)

# obliga a usar C++17 y evita que el compilador "baje" a una versión - 
# - anterior si no encuentra soporte.
set(CMAKE_CXX_STANDARD_REQUIERD ON)

# ubicacion de los ".hpps" que deberá encontrar para compilar
include_directories(
    Src/business
    Src/model
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
    Src/model/TransaccionEnergia.cpp
    
)
