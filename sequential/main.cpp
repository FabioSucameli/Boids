#include "./boids.h"

int main() {
    constexpr int maxPositions = 100000; // Numero massimo di posizioni da processare
    constexpr int NumBoids = 50000;

    // Inizializzazione dei dati dei boids
    BoidData boidData;
    initializeBoids(NumBoids, boidData);

    std::cout << "Test Sequenziale:\n";

    int positionsProcessed = 0; // Variabile per tracciare il numero di posizioni elaborate
    double startTime = omp_get_wtime(); // Inizio timer

    // Ciclo per processare un numero massimo di posizioni
    while (positionsProcessed < maxPositions) {
        updateBoids(boidData); // Aggiorna i boids
        positionsProcessed += static_cast<int>(boidData.posX.size());
    }

    double endTime = omp_get_wtime(); // Fine timer
    double elapsedTime = (endTime - startTime) * 1000; // Convertito in millisecondi

    // Stampa i risultati
    std::cout << "Tempo Sequenziale: " << elapsedTime
              << " ms, Posizioni Processate: " << positionsProcessed
              << ", con: " << NumBoids << " Boids\n";

    return 0;
}