#include "AirObstacle.hpp"
#include <cmath>

AirObstacle::AirObstacle(sf::Vector2f pos, const sf::Texture& tex,
                         float w, float h,
                         float speed, int frames, float animSpd)
    : Obstacle(pos, ObstacleType::Air, tex, w, h, speed, frames, animSpd),
      timeAccum(0.f)
{}

// Ajoute un mouvement sinusoïdal vertical
// au déplacement horizontal hérité
void AirObstacle::update(float dt)
{
    // Exécute la logique de base
    Obstacle::update(dt);

    // Extra: mouvement vertical
    timeAccum   += dt;
    float baseY  = position.y;
    float swoop  = std::sin(timeAccum * 2.6f) * 18.f;
    sprite.setPosition({position.x, baseY + swoop});
    // Note: position.y n'est pas changée intentionnellement
}
