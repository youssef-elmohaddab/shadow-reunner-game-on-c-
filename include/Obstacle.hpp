#ifndef Obstacle_h
#define Obstacle_h

#include <SFML/Graphics.hpp>

class Obstacle
{
public:
    enum class ObstacleType { Ground, Air };

protected:
    sf::Sprite      sprite;
    sf::Vector2f    position;
    ObstacleType    type;

    int     frameCount;
    int     currentFrame;
    float   animTimer;
    float   animSpeed;
    int     frameW;
    int     frameH;

    float   moveSpeed;

public:
    Obstacle(sf::Vector2f pos, ObstacleType m_type,
             const sf::Texture& tex,
             float w, float h,
             float speed,
             int   frames  = 3,
             float animSpd = 0.12f);

    virtual ~Obstacle() = default;

    virtual void          update(float dt);
    virtual void          render(sf::RenderWindow& window);
    virtual ObstacleType  getType()   const;

    float           getX()      const;
    float           getY()      const;
    void            setX(float x);
    sf::FloatRect   getBounds() const;
};

#endif
