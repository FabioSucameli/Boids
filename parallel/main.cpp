//#include <SFML/Graphics.hpp>
#include <omp.h>
#include "./boids.h"
#include <iostream>
#include <sstream>
#include <atomic>


int main() {
    // Creazione della finestra disattivata per stampare le posizioni dei Boids
    // sf::RenderWindow window(sf::VideoMode(800, 600), "Boids Simulation with SFML");

    BoidData boidData;
    initializeBoids(5000, boidData);

    // Numero di thread configurato
    const int numThreads = 8;
    omp_set_num_threads(numThreads);

    // Inizio del timer con OpenMP
    double startTime = omp_get_wtime();

    const int maxPositions = 10000; // Numero massimo di posizioni da stampare
    std::atomic<int> positionsPrinted(0); // Variabile atomica per conteggio thread-safe

    while (/*window.isOpen()*/ true) { // Loop senza la finestra
        // Evento della finestra disattivato
        /*
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }
        */

        // Aggiorna i boids
        updateBoids(boidData);

        // Calcola quante posizioni possono essere stampate in questo ciclo
        int remainingPositions = maxPositions - positionsPrinted.load();
        const int positionsToPrint = std::min(remainingPositions, static_cast<int>(boidData.posX.size()));

        // Stampa le posizioni rimanenti in parallelo
        if (positionsToPrint > 0) {
            printPositions(boidData, positionsToPrint);
            positionsPrinted += positionsToPrint;
        }

        // Uscita se il numero massimo di posizioni è raggiunto
        if (positionsPrinted.load() >= maxPositions) {
            break;
        }
    }

    // Fine del timer
    double endTime = omp_get_wtime();
    double elapsedTime = (endTime - startTime) * 1000; // Convertito in millisecondi

    std::cout << "Tempo per processare " << maxPositions << " posizioni: " << elapsedTime << " millisecondi\n";

    return 0;
}
