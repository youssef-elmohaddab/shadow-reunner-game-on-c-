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

};

#endif
