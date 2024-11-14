#include <SFML/Graphics.hpp>
#include <chrono>
#include "./boids.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Boids Simulation with SFML");

    std::vector<Vec2> positions;
    std::vector<Vec2> velocities;
    std::vector<bool> isScoutGroup1;
    std::vector<bool> isScoutGroup2;

    initializeBoids(50, positions, velocities, isScoutGroup1, isScoutGroup2);

    auto startTime = std::chrono::steady_clock::now();
    const int durationInSeconds = 10;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        updateBoids(positions, velocities, isScoutGroup1, isScoutGroup2);
        printPositions(positions);

        window.clear();
        drawBoids(window, positions, velocities);
        window.display();

        auto currentTime = std::chrono::steady_clock::now();
        int elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
        if (elapsedSeconds >= durationInSeconds) {
            window.close();
        }
    }

    return 0;
}
