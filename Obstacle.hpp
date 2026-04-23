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

    // Animation
    int     frameCount;
    int     currentFrame;
    float   animTimer;
    float   animSpeed;      // seconds per frame
    int     frameW;
    int     frameH;

    // Movement
    float   moveSpeed;      // leftward drift speed (px/s)

public:
    Obstacle(sf::Vector2f pos, ObstacleType m_type,
             const sf::Texture& tex,
             float w, float h,
             float speed,
             int   frames  = 3,
             float animSpd = 0.12f);

    virtual ~Obstacle() = default;

    // ── Polymorphic interface ──────────────────────────────────────────────
    // Subclasses may override to add type-specific behaviour (e.g. swooping
    // flight for AirObstacle, burrowing for future GroundObstacle variants).
    virtual void          update(float dt);
    virtual void          render(sf::RenderWindow& window);
    virtual ObstacleType  getType()   const;
    // ──────────────────────────────────────────────────────────────────────

    float           getX()      const;
    float           getY()      const;
    void            setX(float x);
    sf::FloatRect   getBounds() const;
};

#endif
