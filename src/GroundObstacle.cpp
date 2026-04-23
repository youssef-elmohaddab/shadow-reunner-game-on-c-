#include "../include/GroundObstacle.hpp"

GroundObstacle::GroundObstacle(sf::Vector2f pos, const sf::Texture& tex,
                               float w, float h,
                               float speed, int frames, float animSpd)
    : Obstacle(pos, ObstacleType::Ground, tex, w, h, speed, frames, animSpd)
{}
