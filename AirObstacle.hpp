#ifndef AirObstacle_h
#define AirObstacle_h

#include "Obstacle.hpp"

class AirObstacle : public Obstacle
{
public:
    AirObstacle(sf::Vector2f pos, const sf::Texture& tex,
                float w, float h,
                float speed   = 160.f,
                int   frames  = 3,
                float animSpd = 0.10f);

    void update(float dt) override;

private:
    float timeAccum;
};

#endif
