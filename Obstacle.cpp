#include "Obstacle.hpp"

Obstacle::Obstacle(sf::Vector2f pos, ObstacleType m_type,
                   const sf::Texture& tex,
                   float w, float h,
                   float speed,
                   int   frames,
                   float animSpd)
    : sprite(tex),
      position(pos),
      type(m_type),
      frameCount(frames),
      currentFrame(0),
      animTimer(0.f),
      animSpeed(animSpd),
      moveSpeed(speed)
{
    frameW = static_cast<int>(tex.getSize().x) / frameCount;
    frameH = static_cast<int>(tex.getSize().y);

    float sx = (frameW > 0) ? w / static_cast<float>(frameW) : 1.f;
    float sy = (frameH > 0) ? h / static_cast<float>(frameH) : 1.f;
    sprite.setScale({sx, sy});

    sprite.setOrigin({0.f, static_cast<float>(frameH)});
    sprite.setTextureRect(sf::IntRect({0, 0}, {frameW, frameH}));
    sprite.setPosition(position);
}

void Obstacle::update(float dt)
{
    animTimer += dt;
    if (animTimer >= animSpeed)
    {
        animTimer    = 0.f;
        currentFrame = (currentFrame + 1) % frameCount;
        sprite.setTextureRect(
            sf::IntRect({currentFrame * frameW, 0}, {frameW, frameH}));
    }

    position.x -= moveSpeed * dt;
    sprite.setPosition(position);
}

void Obstacle::render(sf::RenderWindow& w)
{
    w.draw(sprite);
}

Obstacle::ObstacleType Obstacle::getType() const
{
    return type;
}

float Obstacle::getX() const { return position.x; }
float Obstacle::getY() const { return position.y; }

sf::FloatRect Obstacle::getBounds() const
{
    return sprite.getGlobalBounds();
}

void Obstacle::setX(float x)
{
    position.x = x;
    sprite.setPosition(position);
}
