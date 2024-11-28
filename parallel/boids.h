#ifndef BOIDS_H
#define BOIDS_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <atomic>
#ifdef _OPENMP
#include <omp.h>
#endif

// Parametri generali del sistema (costanti)
const float VISUAL_RANGE = 100.0f;      // Raggio entro cui i boids vedono gli altri boids
const float PROTECTED_RANGE = 20.0f;    // Raggio minimo per evitare collisioni
const float CENTERING_FACTOR = 0.001f;  // Influenza del movimento verso il centro del gruppo
const float AVOID_FACTOR = 0.05f;       // Forza per evitare collisioni
const float MATCHING_FACTOR = 0.05f;    // Influenza per allineare la velocità al gruppo
const float TURN_FACTOR = 0.2f;         // Forza di correzione ai bordi
const float MIN_SPEED = 2.0f;           // Velocità minima del boid
const float MAX_SPEED = 3.0f;           // Velocità massima del boid

constexpr size_t CACHE_LINE_SIZE = 64;  //allineamento struttura per evitare false sharing

// Struttura per gestire i dati dei boids come SoA (Structure of Arrays)
struct alignas(CACHE_LINE_SIZE) BoidData {
    std::vector<float> posX;            // Posizioni X di tutti i boids
    std::vector<float> posY;            // Posizioni Y di tutti i boids
    std::vector<float> velX;            // Velocità X di tutti i boids
    std::vector<float> velY;            // Velocità Y di tutti i boids
    std::vector<bool> isScoutGroup1;    // Gruppo 1 (scout movimento in basso a destra) per ogni boid
    std::vector<bool> isScoutGroup2;    // Gruppo 2 (scout movimento in basso a sinistra) per ogni boid
};

// Ridimensiona i vettori della struttura BoidData
void resizeBoidData(BoidData& boidData, size_t numBoids);

// Inizializza i boids con posizioni, velocità e appartenenza ai gruppi casuali
void initializeBoids(int numBoids, BoidData& boidData);

// Aggiorna posizioni e velocità dei boids
void updateBoids(BoidData& boidData);

// Disegna i boids nella finestra
void drawBoids(sf::RenderWindow& window, const BoidData& boidData);

// Stampa le posizioni dei boids
void printPositions(const BoidData& boidData, int positionsToPrint);

// Controlla se un boid è ai bordi della finestra e modifica la velocità per tenerlo all'interno
inline void checkEdges(float& posX, float& posY, float& velX, float& velY);

// Limita la velocità del boid ai valori MIN_SPEED e MAX_SPEED
inline void enforceSpeedLimits(float& velX, float& velY);

// Normalizza un vettore 2D (x, y) per avere lunghezza unitaria
inline void normalize(float& x, float& y);

#endif // BOIDS_H
