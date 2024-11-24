#include <SFML/Graphics.hpp>
#include <chrono>
#include "./boids.h"
#include <iostream>

int main() {
    // Creazione della finestra SFML disattivata per stampare le posizioni dei Boids
    // sf::RenderWindow window(sf::VideoMode(800, 600), "Boids Simulation with SFML");

    BoidData boidData;
    initializeBoids(5000, boidData);

    // Inizio del timer con std::chrono
    auto startTime = std::chrono::high_resolution_clock::now();

    const int maxPositions = 10000; // Numero massimo di posizioni da stampare
    int positionsPrinted = 0;

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
        int remainingPositions = maxPositions - positionsPrinted;
        int positionsToPrint = std::min(remainingPositions, static_cast<int>(boidData.posX.size()));

        // Stampa le posizioni rimanenti
        printPositions(boidData, positionsToPrint);
        positionsPrinted += positionsToPrint;

        // Uscita se il numero massimo di posizioni è raggiunto
        if (positionsPrinted >= maxPositions) {
            break;
        }
    }

    // Fine del timer
    auto endTime = std::chrono::high_resolution_clock::now();
    auto elapsedMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    std::cout << "Tempo per processare " << maxPositions << " posizioni: " << elapsedMilliseconds << " millisecondi\n";

    return 0;
}
