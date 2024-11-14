#ifndef BOIDS_H
#define BOIDS_H

#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <cstdlib>

struct Vec2 {
    float x, y;
    Vec2() : x(0), y(0) {}
    Vec2(float x, float y) : x(x), y(y) {}
    Vec2 operator+(const Vec2& other) const { return Vec2(x + other.x, y + other.y); }
    Vec2 operator-(const Vec2& other) const { return Vec2(x - other.x, y - other.y); }
    Vec2 operator*(float scalar) const { return Vec2(x * scalar, y * scalar); }
    Vec2 operator/(float scalar) const { return Vec2(x / scalar, y / scalar); }
    float length() const { return std::sqrt(x * x + y * y); }
    Vec2 normalized() const { return *this / length(); }
};

// Parametri generali del sistema
const float VISUAL_RANGE = 100.0f;
const float PROTECTED_RANGE = 20.0f;
const float CENTERING_FACTOR = 0.001f;
const float AVOID_FACTOR = 0.05f;
const float MATCHING_FACTOR = 0.05f;
const float TURN_FACTOR = 0.2f;
const float MIN_SPEED = 2.0f;
const float MAX_SPEED = 3.0f;

// Funzioni del sistema di boids
void initializeBoids(int numBoids, std::vector<Vec2>& positions, std::vector<Vec2>& velocities,
                     std::vector<bool>& isScoutGroup1, std::vector<bool>& isScoutGroup2);

void updateBoids(std::vector<Vec2>& positions, std::vector<Vec2>& velocities, const std::vector<bool>& isScoutGroup1,
                 const std::vector<bool>& isScoutGroup2);

void drawBoids(sf::RenderWindow& window, const std::vector<Vec2>& positions, const std::vector<Vec2>& velocities);

void printPositions(const std::vector<Vec2>& positions);

// Funzioni di supporto
void checkEdges(Vec2& position, Vec2& velocity);
void enforceSpeedLimits(Vec2& velocity);

#endif // BOIDS_H
