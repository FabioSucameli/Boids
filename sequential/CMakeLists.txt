cmake_minimum_required(VERSION 3.8)
project(Boids)

set(CMAKE_CXX_STANDARD 11)

# Percorso relativo a SFML
set(SFML_DIR "${CMAKE_SOURCE_DIR}/SFML/lib/cmake/SFML")

# Pacchetti SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Aggiungi il percorso degli header SFML
include_directories("${CMAKE_SOURCE_DIR}/SFML/include")

# Eseguibile principale
add_executable(Boids main.cpp boids.cpp)

# Collegamento librerie SFML al progetto
target_link_libraries(Boids sfml-graphics sfml-window sfml-system)
