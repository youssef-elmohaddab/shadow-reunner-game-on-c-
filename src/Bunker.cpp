#include "../include/Bunker.hpp"

Bunker::Bunker(const sf::Texture& tex, sf::Vector2f pos, float w, float h)
    : sprite(tex), position(pos), reached(false)
{
    float texW = static_cast<float>(tex.getSize().x);
    float texH = static_cast<float>(tex.getSize().y);
    if (texW > 0.f && texH > 0.f)
        sprite.setScale({w / texW, h / texH});

    sprite.setOrigin({0.f, static_cast<float>(tex.getSize().y)});
    sprite.setPosition(position);
}

void          Bunker::update(float ) {}
void          Bunker::render(sf::RenderWindow& w)
{
    w.draw(sprite);
}
sf::FloatRect Bunker::getBounds() const
{
    return sprite.getGlobalBounds();
}
bool          Bunker::isReached() const
{
    return reached;
}
void          Bunker::setReached()
{
    reached = true;
}
float         Bunker::getX()      const
{
    return position.x;
}
