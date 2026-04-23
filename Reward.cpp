#include "Reward.hpp"

Reward::Reward(const sf::Texture& tex, sf::Vector2f pos, float size)
    : sprite(tex), collected(false)
{
    float texW = static_cast<float>(tex.getSize().x);
    float texH = static_cast<float>(tex.getSize().y);
    if (texW > 0.f && texH > 0.f)
        sprite.setScale({size / texW, size / texH});

    sprite.setOrigin({0.f, static_cast<float>(tex.getSize().y)});
    sprite.setPosition(pos);
}

void          Reward::update(float ) {}

void          Reward::render(sf::RenderWindow& window)
{
    if (!collected) window.draw(sprite);
}

bool          Reward::isCollected() const { return collected; }
void          Reward::collect()          { collected = true; }
sf::FloatRect Reward::getBounds()   const { return sprite.getGlobalBounds(); }
