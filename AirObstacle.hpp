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

    // Polymorphic override: air obstacles add a gentle vertical sine-wave
    // (swooping motion) on top of the base leftward drift.
    void update(float dt) override;

private:
    float timeAccum;   // accumulates elapsed time for the sine wave
};

#endif
