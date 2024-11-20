#include <SFML/Graphics.hpp>
#include <chrono>
#include "./boids.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Boids Simulation with SFML");

    BoidData boidData;
    initializeBoids(50, boidData);

    auto startTime = std::chrono::steady_clock::now();
    const int durationInSeconds = 10;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        updateBoids(boidData);
        //printPositions(boidData);

        window.clear();
        drawBoids(window, boidData);
        window.display();

        auto currentTime = std::chrono::steady_clock::now();
        int elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
        if (elapsedSeconds >= durationInSeconds) {
            window.close();
        }
    }

    return 0;
}
