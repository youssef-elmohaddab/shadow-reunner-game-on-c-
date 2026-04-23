#include "AirObstacle.hpp"
#include <cmath>

AirObstacle::AirObstacle(sf::Vector2f pos, const sf::Texture& tex,
                         float w, float h,
                         float speed, int frames, float animSpd)
    : Obstacle(pos, ObstacleType::Air, tex, w, h, speed, frames, animSpd),
      timeAccum(0.f)
{}

// Polymorphic override ─────────────────────────────────────────────────────
// Adds a vertical sine-wave swoop (amplitude 18 px, period ~2.4 s) on top of
// the inherited horizontal drift and sprite animation.
void AirObstacle::update(float dt)
{
    // Run the base logic (animation frames + leftward move)
    Obstacle::update(dt);

    // Extra: vertical swooping
    timeAccum   += dt;
    float baseY  = position.y;          // updated by Obstacle::update above
    float swoop  = std::sin(timeAccum * 2.6f) * 18.f;
    sprite.setPosition({position.x, baseY + swoop});
    // Note: position.y is intentionally NOT changed so the swoop stays centred.
}
