#ifndef GroundObstacle_h
#define GroundObstacle_h

#include "Obstacle.hpp"

class GroundObstacle : public Obstacle
{
public:
    GroundObstacle(sf::Vector2f pos, const sf::Texture& tex,
                   float w, float h,
                   float speed   = 80.f,
                   int   frames  = 3,
                   float animSpd = 0.12f);

    // Uses the base Obstacle::update / render — explicitly noted for clarity.
    // Override here if ground-specific behaviour is needed in the future.
};

#endif
