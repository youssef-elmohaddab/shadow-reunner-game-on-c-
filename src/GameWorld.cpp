#include "../include/GameWorld.hpp"
#include "../include/GroundObstacle.hpp"
#include "../include/AirObstacle.hpp"
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cmath>
#include <algorithm>

sf::Texture GameWorld::loadTexture(const std::string& path)
{
    sf::Texture tex;
    if (!tex.loadFromFile(path))
        throw std::runtime_error("GameWorld: failed to load texture: " + path);
    return tex;
}

GameWorld::GameWorld(sf::RenderWindow& window, int level)
    : bgTexture    (loadTexture("data/images/background.png")),
      groundObsTex (loadTexture("data/images/groundObstacle.png")),
      airObsTex    (loadTexture("data/images/airObstacle.png")),
      rewardTex    (loadTexture("data/images/reward.png")),
      bgSprite1    (bgTexture),
      bgSprite2    (bgTexture),
      playerTexture(loadTexture("data/images/player.png")),
      bunkerTexture(loadTexture("data/images/bunker.png")),
      player       ({100.f, window.getSize().y * 0.85f}, playerTexture),
      bunker       (nullptr),
      groundY      (window.getSize().y * 0.85f),
      hitCooldown  (0.f),
      rewardsCollected(0),
      levelCfg     (makeLevelConfig(level))
{
    const float refW = 1200.f;
    const float refH = 800.f;
    const float sw   = static_cast<float>(window.getSize().x);
    const float sh   = static_cast<float>(window.getSize().y);

    // Icône de fenêtre — non fatal si manquant
    try {
        if (!icon.loadFromFile("data/images/icon.png"))
            throw std::runtime_error("icon not found");
        window.setIcon(icon);
    } catch (const std::exception& e) {
        std::cerr << "Warning: " << e.what() << "\n";
    }

    obstacleW  = 48.f  * (sw / refW);
    obstacleH  = 64.f  * (sh / refH);
    rewardSize = 32.f  * (sh / refH);
    bunkerW    = 32.f  * (sw / refW);
    bunkerH    = 32.f  * (sh / refH);
    playerSpriteH = 128.f * (sh / refH);

    minSpawnDistance = levelCfg.spawnDensity;

    // Mise à l'échelle de l'arrière-plan
    float scaleY = sh / static_cast<float>(bgTexture.getSize().y);
    bgSprite1.setScale({scaleY, scaleY});
    bgSprite2.setScale({scaleY, scaleY});
    bgWidth = static_cast<float>(bgTexture.getSize().x) * scaleY;
    bgSprite1.setPosition({0.f, 0.f});
    bgSprite2.setPosition({bgWidth, 0.f});

    bunker = new Bunker(bunkerTexture,
                        {levelCfg.goalX, groundY - 32.f},
                        bunkerW, bunkerH);

    std::srand(static_cast<unsigned>(std::time(nullptr)));
    for (int i = 0; i < 5; ++i)
        spawnObstacleIfNeeded();
}

GameWorld::~GameWorld()
{
    for (auto* obs : obstacles) delete obs;
    obstacles.clear();
    delete bunker;
    bunker = nullptr;
}

Player& GameWorld::getPlayer()             { return player; }
int     GameWorld::getRewardsCollected() const { return rewardsCollected; }

GameWorld::HitResult GameWorld::update(float dt)
{
    player.update(dt);

    // Chaque sous-classe Obstacle exécute son propre update()
    for (auto* obs : obstacles)
        obs->update(dt);

    float viewLeft = player.getPosition().x - 400.f;
    float offset   = std::fmod(viewLeft, bgWidth);
    bgSprite1.setPosition({viewLeft - offset,           0.f});
    bgSprite2.setPosition({viewLeft - offset + bgWidth, 0.f});

    spawnObstacleIfNeeded();
    cleanupOffscreen();

    sf::FloatRect ph = player.getHitbox();

    // Collecte de récompenses
    for (auto& rew : rewards)
        if (!rew.isCollected() && rew.getBounds().findIntersection(ph))
        {
            rew.collect();
            ++rewardsCollected;
        }

    // Condition de victoire (bunker)
    if (bunker && !bunker->isReached())
    {
        float playerCX   = ph.position.x + ph.size.x * 0.5f;
        float bunkerLeft = levelCfg.goalX;
        float bunkerRight= levelCfg.goalX + bunkerW + 60.f;
        bool byPosition  = (playerCX >= bunkerLeft - 20.f && playerCX <= bunkerRight);
        bool byBounds    = bunker->getBounds().size.x > 0.f &&
                           bunker->getBounds().findIntersection(ph).has_value();
        if (byPosition || byBounds)
        {
            bunker->setReached();
            return HitResult::Won;
        }
    }

    // Collision avec obstacle
    HitResult result = HitResult::None;
    if (hitCooldown > 0.f)
    {
        hitCooldown -= dt;
    }
    else
    {
        for (auto* obs : obstacles)
        {
            sf::FloatRect ob = obs->getBounds();
            float ox = ob.size.x * 0.20f;
            float oy = ob.size.y * 0.10f;
            sf::FloatRect obHitbox(
                { ob.position.x + ox,     ob.position.y + oy },
                { ob.size.x - ox * 2.f,   ob.size.y - oy * 2.f }
            );

            if (obHitbox.findIntersection(ph))
            {
                player.loseLife();
                player.knockBack();
                hitCooldown = kHitCooldown;
                result = (player.getLives() <= 0)
                         ? HitResult::Dead
                         : HitResult::Hurt;
                break;
            }
        }
    }

    return result;
}

static void drawRect(sf::RenderWindow& w, sf::FloatRect r, sf::Color c)
{
    sf::RectangleShape s({r.size.x, r.size.y});
    s.setPosition({r.position.x, r.position.y});
    s.setFillColor(sf::Color::Transparent);
    s.setOutlineColor(c);
    s.setOutlineThickness(2.f);
    w.draw(s);
}

void GameWorld::render(sf::RenderWindow& window)
{
    window.draw(bgSprite1);
    window.draw(bgSprite2);
    // Chaque sous-classe se rend elle-même
    for (auto* obs : obstacles) obs->render(window);
    for (auto& rew : rewards)   rew.render(window);
    if (bunker) bunker->render(window);
    player.render(window);

    if (debugHitboxes)
    {
        drawRect(window, player.getHitbox(), sf::Color::Green);
        drawRect(window, player.getBounds(), sf::Color::Yellow);
        for (auto* obs : obstacles)
        {
            sf::FloatRect ob = obs->getBounds();
            float ox = ob.size.x * 0.20f;
            float oy = ob.size.y * 0.10f;
            sf::FloatRect trimmed(
                { ob.position.x + ox,   ob.position.y + oy },
                { ob.size.x - ox * 2.f, ob.size.y - oy * 2.f }
            );
            drawRect(window, ob,      sf::Color::White);
            drawRect(window, trimmed, sf::Color::Red);
        }
    }
}

void GameWorld::spawnObstacleIfNeeded()
{
    float spawnX = player.getPosition().x + 900.f;
    if (spawnX >= levelCfg.goalX - 400.f) return;

    if (!obstacles.empty())
    {
        float lastX = obstacles.back()->getX();
        if (spawnX - lastX < minSpawnDistance) return;
        spawnX = lastX + minSpawnDistance
                 + static_cast<float>(std::rand() % 300);
    }

    if (spawnX >= levelCfg.goalX - 400.f) return;

    if (std::rand() % 2 == 0)
    {
        obstacles.push_back(
            new GroundObstacle(
                {spawnX, groundY},
                groundObsTex,
                obstacleW, obstacleH,
                levelCfg.groundObsSpeed));
    }
    else
    {
        float crouchHitboxTop = groundY - playerSpriteH * 0.45f;
        float gap             = 8.f * (obstacleH / 64.f);
        float airY            = crouchHitboxTop - gap;

        obstacles.push_back(
            new AirObstacle(
                {spawnX, airY},
                airObsTex,
                obstacleW, obstacleH,
                levelCfg.airObsSpeed));
    }

    if (std::rand() % 3 == 0)
        rewards.emplace_back(rewardTex,
                             sf::Vector2f(spawnX + 60.f,
                                          groundY - rewardSize * 3.5f),
                             rewardSize);
}

void GameWorld::cleanupOffscreen()
{
    auto it = obstacles.begin();
    while (it != obstacles.end())
    {
        float rightEdge = (*it)->getX() + (*it)->getBounds().size.x;
        if (rightEdge < 0.f)
        {
            delete *it;
            it = obstacles.erase(it);
        }
        else ++it;
    }

    rewards.erase(
        std::remove_if(rewards.begin(), rewards.end(),
            [](const Reward& r) {
                return r.isCollected() ||
                       r.getBounds().position.x + r.getBounds().size.x < 0.f;
            }),
        rewards.end());
}
