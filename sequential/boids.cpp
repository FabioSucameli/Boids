#include "./boids.h"

// Inizializza i boids con posizioni, velocità e appartenenza ai gruppi casuali
void initializeBoids(int numBoids, std::vector<Vec2>& positions, std::vector<Vec2>& velocities,
                     std::vector<bool>& isScoutGroup1, std::vector<bool>& isScoutGroup2) {
    positions.resize(numBoids);
    velocities.resize(numBoids);
    isScoutGroup1.resize(numBoids, false);
    isScoutGroup2.resize(numBoids, false);

    for (int i = 0; i < numBoids; ++i) {
        positions[i] = Vec2(rand() % 800, rand() % 600);
        velocities[i] = Vec2((rand() % 100) / 50.0f - 1, (rand() % 100) / 50.0f - 1);
        isScoutGroup1[i] = (rand() % 2 == 0);
        isScoutGroup2[i] = !isScoutGroup1[i];
    }
}

// Aggiorna lo stato di ciascun boid
void updateBoids(std::vector<Vec2>& positions, std::vector<Vec2>& velocities, const std::vector<bool>& isScoutGroup1,
                 const std::vector<bool>& isScoutGroup2) {
    int numBoids = positions.size();
    for (int i = 0; i < numBoids; ++i) {
        Vec2 xpos_avg(0, 0), xvel_avg(0, 0);
        int neighboring_boids = 0;
        Vec2 close(0, 0);

        for (int j = 0; j < numBoids; ++j) {
            if (i == j) continue;
            Vec2 diff = positions[i] - positions[j];
            float dist_squared = diff.x * diff.x + diff.y * diff.y;

            if (dist_squared < PROTECTED_RANGE * PROTECTED_RANGE) {
                close = close + diff;
            } else if (dist_squared < VISUAL_RANGE * VISUAL_RANGE) {
                xpos_avg = xpos_avg + positions[j];
                xvel_avg = xvel_avg + velocities[j];
                neighboring_boids++;
            }
        }

        if (neighboring_boids > 0) {
            xpos_avg = xpos_avg / neighboring_boids;
            xvel_avg = xvel_avg / neighboring_boids;
            velocities[i] = velocities[i]
                            + (xpos_avg - positions[i]) * CENTERING_FACTOR
                            + (xvel_avg - velocities[i]) * MATCHING_FACTOR;
        }

        velocities[i] = velocities[i] + close * AVOID_FACTOR;
        checkEdges(positions[i], velocities[i]);
        enforceSpeedLimits(velocities[i]);
        positions[i] = positions[i] + velocities[i];
    }
}

// Disegna i boids sulla finestra
void drawBoids(sf::RenderWindow& window, const std::vector<Vec2>& positions, const std::vector<Vec2>& velocities) {
    for (int i = 0; i < positions.size(); ++i) {
        sf::ConvexShape triangle;
        triangle.setPointCount(3);
        triangle.setPoint(0, sf::Vector2f(0, 0));
        triangle.setPoint(1, sf::Vector2f(-5, -10));
        triangle.setPoint(2, sf::Vector2f(5, -10));

        triangle.setPosition(positions[i].x, positions[i].y);
        float angle = std::atan2(velocities[i].y, velocities[i].x) * 180 / 3.14159265;
        triangle.setRotation(angle + 90);
        triangle.setFillColor(sf::Color::Cyan);

        window.draw(triangle);
    }
}

// Stampa le posizioni dei boids
void printPositions(const std::vector<Vec2>& positions) {
    for (const auto& pos : positions) {
        std::cout << "Boid Position: (" << pos.x << ", " << pos.y << ")\n";
    }
}

// Controlla i bordi della finestra e aggiorna la velocità
void checkEdges(Vec2& position, Vec2& velocity) {
    if (position.x < 10) velocity.x += TURN_FACTOR;
    if (position.x > 790) velocity.x -= TURN_FACTOR;
    if (position.y < 10) velocity.y += TURN_FACTOR;
    if (position.y > 590) velocity.y -= TURN_FACTOR;
}

// Applica i limiti di velocità
void enforceSpeedLimits(Vec2& velocity) {
    float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (speed < MIN_SPEED) {
        velocity = velocity / speed * MIN_SPEED;
    } else if (speed > MAX_SPEED) {
        velocity = velocity / speed * MAX_SPEED;
    }
}
