#include "./boids.h"

// Normalizza un vettore 2D, portandolo a lunghezza unitaria (se non è nullo)
inline void normalize(float& x, float& y) {
    float length = std::sqrt(x * x + y * y);
    if (length > 0) {
        x /= length;
        y /= length;
    }
}


// Controlla i bordi della finestra e corregge la velocità per mantenere il boid all'interno
inline void checkEdges(float& posX, float& posY, float& velX, float& velY) {
    if (posX < 10) velX += TURN_FACTOR;
    if (posX > 790) velX -= TURN_FACTOR;
    if (posY < 10) velY += TURN_FACTOR;
    if (posY > 590) velY -= TURN_FACTOR;
}

// Limita la velocità del boid ai valori minimi e massimi
inline void enforceSpeedLimits(float& velX, float& velY) {
    float speed = std::sqrt(velX * velX + velY * velY);
    if (speed < MIN_SPEED) {
        velX = velX / speed * MIN_SPEED;
        velY = velY / speed * MIN_SPEED;
    } else if (speed > MAX_SPEED) {
        velX = velX / speed * MAX_SPEED;
        velY = velY / speed * MAX_SPEED;
    }
}


// Ridimensiona i vettori della struttura BoidData per contenere un certo numero di boids
void resizeBoidData(BoidData& boidData, size_t numBoids) {
    boidData.posX.resize(numBoids);
    boidData.posY.resize(numBoids);
    boidData.velX.resize(numBoids);
    boidData.velY.resize(numBoids);
    boidData.isScoutGroup1.resize(numBoids);
    boidData.isScoutGroup2.resize(numBoids);
}

// Inizializza i boids con posizioni e velocità casuali, assegnandoli a gruppi scout casuali
void initializeBoids(int numBoids, BoidData& boidData) {
    resizeBoidData(boidData, numBoids);

    for (size_t i = 0; i < numBoids; ++i) {
        boidData.posX[i] = static_cast<float>(rand() % 800);                // Posizione X casuale tra 0 e 800
        boidData.posY[i] = static_cast<float>(rand() % 600);                // Posizione Y casuale tra 0 e 600
        boidData.velX[i] = static_cast<float>((rand() % 100) / 50.0 - 1);   // Velocità X casuale tra -1 e 1
        boidData.velY[i] = static_cast<float>((rand() % 100) / 50.0 - 1);   // Velocità Y casuale tra -1 e 1
        boidData.isScoutGroup1[i] = (rand() % 2 == 0);                      // Assegna casualmente al gruppo 1
        boidData.isScoutGroup2[i] = !boidData.isScoutGroup1[i];             // Opposto del gruppo 1
    }
}

// Aggiorna le posizioni e velocità dei boids applicando le regole di interazione
void updateBoids(BoidData& boidData) {
    size_t numBoids = boidData.posX.size();
    std::vector<float> tempVelX(numBoids); // Velocità temporanea X
    std::vector<float> tempVelY(numBoids); // Velocità temporanea Y

    // Bias di direzione per i gruppi scout
    float biasDir1X = 1.0f, biasDir1Y = 0.5f; // Bias direzionale gruppo 1
    float biasDir2X = -0.5f, biasDir2Y = 1.0f; // Bias direzionale gruppo 2
    normalize(biasDir1X, biasDir1Y);
    normalize(biasDir2X, biasDir2Y);

    // Calcolo delle nuove velocità con ottimizzazioni
#pragma omp parallel for schedule(static, 32) default(none) \
    shared(boidData, tempVelX, tempVelY, biasDir1X, biasDir1Y, biasDir2X, biasDir2Y, numBoids)
    for (size_t i = 0; i < numBoids; ++i) {
        float xpos_avg = 0, ypos_avg = 0, xvel_avg = 0, yvel_avg = 0;
        int neighboring_boids = 0; // Numero di boids vicini
        float closeX = 0, closeY = 0; // Movimento per evitare collisioni

        // Ottimizzazione: memorizza le proprietà del boid corrent
        float posX_i = boidData.posX[i]; // Posizione X del boid corrente
        float posY_i = boidData.posY[i]; // Posizione Y del boid corrente
        float velX_i = boidData.velX[i]; // Velocità X del boid corrente
        float velY_i = boidData.velY[i]; // Velocità Y del boid corrente

        for (size_t j = 0; j < numBoids; ++j) {
            if (i == j) continue; // Salta il confronto del boid con sé stesso

            // Distanza tra boid[i] e boid[j]
            float dx = posX_i - boidData.posX[j];
            float dy = posY_i - boidData.posY[j];
            float dist_squared = dx * dx + dy * dy;

            // Evita collisioni se entro il raggio protetto
            if (dist_squared < PROTECTED_RANGE * PROTECTED_RANGE) {
                closeX += dx; // Allontana il boid corrente
                closeY += dy;
            // Regole di interazione entro il raggio visivo    
            } else if (dist_squared < VISUAL_RANGE * VISUAL_RANGE) {
                xpos_avg += boidData.posX[j];
                ypos_avg += boidData.posY[j];
                xvel_avg += boidData.velX[j];
                yvel_avg += boidData.velY[j];
                neighboring_boids++;
            }
        }
        // Se ci sono boids vicini, calcola la media e aggiorna la velocità
        if (neighboring_boids > 0) {
            xpos_avg /= neighboring_boids;
            ypos_avg /= neighboring_boids;
            xvel_avg /= neighboring_boids;
            yvel_avg /= neighboring_boids;

            // Regole: centering e matching
            tempVelX[i] = velX_i
                        + (xpos_avg - posX_i) * CENTERING_FACTOR
                        + (xvel_avg - velX_i) * MATCHING_FACTOR;
            tempVelY[i] = velY_i
                        + (ypos_avg - posY_i) * CENTERING_FACTOR
                        + (yvel_avg - velY_i) * MATCHING_FACTOR;
        }

        // Evita collisioni
        tempVelX[i] += closeX * AVOID_FACTOR;
        tempVelY[i] += closeY * AVOID_FACTOR;

        // Aggiungi bias di gruppo
        if (boidData.isScoutGroup1[i]) {
            tempVelX[i] += biasDir1X * CENTERING_FACTOR;
            tempVelY[i] += biasDir1Y * CENTERING_FACTOR;
        } else if (boidData.isScoutGroup2[i]) {
            tempVelX[i] += biasDir2X * CENTERING_FACTOR;
            tempVelY[i] += biasDir2Y * CENTERING_FACTOR;
        }
    }

    // Secondo passaggio: aggiorna velocità e posizioni
#pragma omp parallel for schedule(static, 32) default(none) \
    shared(boidData, tempVelX, tempVelY, numBoids)
    for (size_t i = 0; i < numBoids; ++i) {
        checkEdges(boidData.posX[i], boidData.posY[i], tempVelX[i], tempVelY[i]);
        enforceSpeedLimits(tempVelX[i], tempVelY[i]);
        boidData.posX[i] += tempVelX[i];
        boidData.posY[i] += tempVelY[i];
        boidData.velX[i] = tempVelX[i];
        boidData.velY[i] = tempVelY[i];
    }
}

//++++++++++++++++++++++++++++++++++++++++++++++++
//+FUNZIONI DI UTILITA', VISUALIZZAZIONE E DEBUG +
//++++++++++++++++++++++++++++++++++++++++++++++++

// Disegna i boids nella finestra fornita
void drawBoids(sf::RenderWindow& window, const BoidData& boidData) {
    for (size_t i = 0; i < boidData.posX.size(); ++i) {
        sf::ConvexShape triangle;
        triangle.setPointCount(3);
        triangle.setPoint(0, sf::Vector2f(0, 0));
        triangle.setPoint(1, sf::Vector2f(-5, -10));
        triangle.setPoint(2, sf::Vector2f(5, -10));

        triangle.setPosition(boidData.posX[i], boidData.posY[i]);
        float angle = std::atan2(boidData.velY[i], boidData.velX[i]) * 180 / 3.14159265;
        triangle.setRotation(angle + 90);
        triangle.setFillColor(sf::Color::Cyan);

        window.draw(triangle);
    }
}

// Stampa le posizioni dei boids nella console
void printPositions(const BoidData& boidData, const int positionsToPrint) {
    // Parallelizzazione con buffer locale per ogni thread
#pragma omp parallel
    {
        std::string buffer; // Buffer temporaneo come stringa
        buffer.reserve(positionsToPrint * 50); // Pre-alloca memoria per migliorare le prestazioni

#pragma omp for schedule(static)
        for (int i = 0; i < positionsToPrint; ++i) {
            buffer += "Boid Position: (" + std::to_string(boidData.posX[i]) + ", " + std::to_string(boidData.posY[i]) + ")\n";
        }

        // Scrittura sincronizzata del buffer globale
#pragma omp critical
        {
            std::cout << buffer;
        }
    }
}
