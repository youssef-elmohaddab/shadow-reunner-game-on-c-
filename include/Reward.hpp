#ifndef Reward_h
#define Reward_h

#include <SFML/Graphics.hpp>

class Reward
{
protected:
    sf::Sprite  sprite;
    bool        collected;

public:
    Reward(const sf::Texture& tex, sf::Vector2f pos, float size = 32.f);
    virtual ~Reward() = default;

    virtual void          update(float dt);
    virtual void          render(sf::RenderWindow& window);
    virtual bool          isCollected()   const;
    virtual void          collect();
    virtual sf::FloatRect getBounds()     const;
};

#endif
