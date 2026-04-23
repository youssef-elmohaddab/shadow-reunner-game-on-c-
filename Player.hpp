#ifndef Player_h
#define Player_h

#include <SFML/Graphics.hpp>

class Player
{
private:
    sf::Sprite      sprite;
    sf::Vector2f    position;
    float           walkSpeed;
    float           runSpeed;
    int             lives;
    bool            isJumping;
    bool            isCrouching;
    bool            isRunning;
    float           jumpInitialSpeed;
    float           velocityY;
    float           gravity;
    float           groundY;
    float           velocityX;
    bool            facingRight;

    enum class AnimState { Idle, Walk, Run, Jump, Crouch };
    AnimState       currentAnim;
    int             frame;
    float           animationTimer;

    void setCurrentAnim(AnimState state);

public:
    Player(sf::Vector2f startPos, const sf::Texture& texture);

    sf::Vector2f    getPosition()    const;
    int             getLives()       const;
    bool            getIsJumping()   const;
    bool            getIsCrouching() const;

    void moveWalkForward();
    void moveWalkBack();
    void moveRunForward();
    void moveRunBack();
    void jump();
    void crouch();
    void standUp();
    void setIdle();
    void loseLife();
    void knockBack();

    void update(float dt);
    void render(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;
    sf::FloatRect getHitbox() const;
};

#endif
