#include "Player.hpp"

struct AnimConfig
{
    int   frames;
    int   row;
    float speed;
    bool  loop;
};

static const AnimConfig configs[] =
{
    {8, 0, 0.12f, true },   // Idle
    {8, 1, 0.09f, true },   // Walk
    {8, 2, 0.06f, true },   // Run
    {8, 3, 0.08f, false},   // Jump
    {3, 4, 0.10f, true }    // Crouch
};

Player::Player(sf::Vector2f startPos, const sf::Texture& texture)
    : sprite(texture),
      walkSpeed(350.f), runSpeed(650.f),
      position(startPos), lives(3),
      isJumping(false), isCrouching(false), isRunning(false),
      jumpInitialSpeed(620.f), velocityY(0.f), gravity(1400.f),
      groundY(startPos.y),
      velocityX(0.f), facingRight(true),
      currentAnim(AnimState::Idle),
      frame(0), animationTimer(0.f)
{
    sprite.setTextureRect(sf::IntRect({0, 0}, {128, 128}));
    sprite.setOrigin(sf::Vector2f(0.f, 128.f));
    sprite.setPosition(position);
}

sf::Vector2f Player::getPosition()    const
{
    return position;
}
int          Player::getLives()       const
{
    return lives;
}
bool         Player::getIsJumping()   const
{
    return isJumping;
}
bool         Player::getIsCrouching() const
{
    return isCrouching;
}

void Player::loseLife()
{
    if (lives > 0) --lives;
}

void Player::knockBack()
{
    position.x  -= 100.f;
    isJumping    = false;
    isCrouching  = false;
    isRunning    = false;
    velocityY    = 0.f;
    velocityX    = 0.f;
    position.y   = groundY;
    setCurrentAnim(AnimState::Idle);
    sprite.setPosition(position);
}

void Player::setCurrentAnim(AnimState state)
{
    if (currentAnim != state)
    {
        currentAnim    = state;
        frame          = 0;
        animationTimer = 0.f;
    }
}

void Player::moveWalkForward()
{
    velocityX   = walkSpeed;
    facingRight = true;
    isRunning   = false;
    if (!isJumping && !isCrouching) setCurrentAnim(AnimState::Walk);
}

void Player::moveWalkBack()
{
    velocityX   = -walkSpeed;
    facingRight = false;
    isRunning   = false;
    if (!isJumping && !isCrouching) setCurrentAnim(AnimState::Walk);
}

void Player::moveRunForward()
{
    velocityX   = runSpeed;
    facingRight = true;
    isRunning   = true;
    if (!isJumping && !isCrouching) setCurrentAnim(AnimState::Run);
}

void Player::moveRunBack()
{
    velocityX   = -runSpeed;
    facingRight = false;
    isRunning   = true;
    if (!isJumping && !isCrouching) setCurrentAnim(AnimState::Run);
}

void Player::setIdle()
{
    // IMPORTANT: never call setIdle while crouching — it would override
    // the crouch animation without actually standing the player up.
    if (isCrouching) return;
    isRunning = false;
    if (!isJumping) setCurrentAnim(AnimState::Idle);
}

void Player::crouch()
{
    if (isJumping) return;   // can't crouch mid-air
    isCrouching = true;
    setCurrentAnim(AnimState::Crouch);
}

void Player::standUp()
{
    if (isCrouching)
    {
        isCrouching = false;
        setCurrentAnim(AnimState::Idle);
    }
}

void Player::jump()
{
    if (!isJumping && !isCrouching)
    {
        isJumping = true;
        velocityY = -jumpInitialSpeed;
        setCurrentAnim(AnimState::Jump);
    }
}

void Player::update(float dt)
{
    position.x += velocityX * dt;
    velocityX = 0.f;

    if (isJumping)
    {
        velocityY  += gravity * dt;
        position.y += velocityY * dt;
        if (position.y >= groundY)
        {
            position.y = groundY;
            isJumping  = false;
            velocityY  = 0.f;
            if (!isCrouching) setCurrentAnim(AnimState::Idle);
        }
    }

    // Animation priority: jump > crouch > walk/run/idle
    if      (isJumping)   setCurrentAnim(AnimState::Jump);
    else if (isCrouching) setCurrentAnim(AnimState::Crouch);

    const AnimConfig& cfg = configs[static_cast<int>(currentAnim)];
    animationTimer += dt;
    if (animationTimer >= cfg.speed)
    {
        animationTimer = 0.f;
        if (++frame >= cfg.frames)
            frame = cfg.loop ? 0 : cfg.frames - 1;
    }

    sprite.setTextureRect(sf::IntRect({frame * 128, cfg.row * 128}, {128, 128}));
    sprite.setScale(facingRight ? sf::Vector2f(1.f, 1.f) : sf::Vector2f(-1.f, 1.f));
    sprite.setPosition(position);
}

void Player::render(sf::RenderWindow& window)
{
    window.draw(sprite);
}

sf::FloatRect Player::getBounds() const
{
    return sprite.getGlobalBounds();
}

sf::FloatRect Player::getHitbox() const
{
    sf::FloatRect b = sprite.getGlobalBounds();

    if (isCrouching)
    {
        float trimSide = b.size.x * 0.20f;   // 20% each side  → 60% wide
        float topCut   = b.size.y * 0.55f;   // cut top 55%    → 45% tall
        return sf::FloatRect(
        { b.position.x + trimSide,      b.position.y + topCut },
        { b.size.x - trimSide * 2.f,    b.size.y - topCut     }
        );
    }
    else
    {
        // Standing / jumping: centre 60% wide, 85% tall (trim 10% top, 5% bot).
        float trimSide = b.size.x * 0.20f;
        float trimTop  = b.size.y * 0.10f;
        float trimBot  = b.size.y * 0.05f;
        return sf::FloatRect(
        { b.position.x + trimSide,      b.position.y + trimTop           },
        { b.size.x - trimSide * 2.f,    b.size.y - trimTop - trimBot     }
        );
    }
}
