#ifndef GameWorld_h
#define GameWorld_h

#include <SFML/Graphics.hpp>
#include <vector>
#include "Player.hpp"
#include "Obstacle.hpp"
#include "Reward.hpp"
#include "Bunker.hpp"
#include "LevelConfig.hpp"

class GameWorld
{
public:
    enum class HitResult { None, Hurt, Dead, Won };

private:
    sf::Texture bgTexture;
    sf::Sprite  bgSprite1, bgSprite2;
    float       bgWidth;

    sf::Texture groundObsTex;
    sf::Texture airObsTex;
    sf::Texture rewardTex;
    sf::Texture playerTexture;
    sf::Texture bunkerTexture;

    sf::Image icon;

    Player                  player;
    std::vector<Obstacle*>  obstacles;   // polymorphic: Ground* or Air*
    std::vector<Reward>     rewards;
    Bunker*                 bunker;

    float groundY;
    float minSpawnDistance;
    float playerSpriteH;

    float obstacleW;
    float obstacleH;
    float rewardSize;
    float bunkerW;
    float bunkerH;

    LevelConfig levelCfg;

    float       hitCooldown;
    int         rewardsCollected;   // NEW: scoreboard counter
    static constexpr float kHitCooldown = 1.5f;

public:
    bool debugHitboxes = false;

private:
    static sf::Texture loadTexture(const std::string& path);
    void spawnObstacleIfNeeded();
    void cleanupOffscreen();

public:
    GameWorld(sf::RenderWindow& window, int level = 0);
    ~GameWorld();

    Player& getPlayer();
    int     getRewardsCollected() const;   // NEW

    HitResult update(float dt);
    void      render(sf::RenderWindow& window);

    // Expose reward texture so Game can draw the HUD scoreboard icon
    const sf::Texture& getRewardTexture() const { return rewardTex; }
};

#endif
