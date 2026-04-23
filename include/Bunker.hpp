#ifndef Bunker_h
#define Bunker_h

#include <SFML/Graphics.hpp>

class Bunker
{
private:
    sf::Sprite      sprite;
    sf::Vector2f    position;
    bool            reached;

public:
    Bunker(const sf::Texture& tex, sf::Vector2f pos, float w, float h);

    void          update(float dt);
    void          render(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    bool          isReached() const;
    void          setReached();
    float         getX()      const;
};

#endif
