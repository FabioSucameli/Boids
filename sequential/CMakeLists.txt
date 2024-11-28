#Note for Building the Project
#This CMake setup is designed to work specifically on Windows systems

cmake_minimum_required(VERSION 3.8)
project(sequential)

# Imposta lo standard C++
set(CMAKE_CXX_STANDARD 17)

# Aggiungi i flag per OpenMP
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

# Percorso relativo a SFML
set(SFML_DIR "${CMAKE_SOURCE_DIR}/SFML/lib/cmake/SFML")

# Pacchetti SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Aggiungi il percorso degli header SFML
include_directories("${CMAKE_SOURCE_DIR}/SFML/include")

# Eseguibile principale
add_executable(sequential main.cpp boids.cpp)

# Collegamento librerie SFML al progetto
target_link_libraries(sequential sfml-graphics sfml-window sfml-system)
