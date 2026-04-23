#ifndef Game_h
#define Game_h

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <optional>
#include <memory>
#include "Menu.hpp"
#include "GameWorld.hpp"
#include "LevelSelect.hpp"

class Game
{
public:
    enum class State { Menu, LevelSelect, Playing, Frozen, Paused, About, Won, Over };

private:
    sf::RenderWindow    window;
    sf::View            camera;
    State               state;

    Menu            menu;
    LevelSelect     levelSelect;
    GameWorld*      world;

    int             currentLevel;
    int             unlockedLevels;
    static constexpr int kTotalLevels = 10;

    sf::Font        uiFont;

    // ── HUD ───────────────────────────────────────────────────────────────
    sf::Text                    timerText;
    sf::RectangleShape          progressBg;
    sf::RectangleShape          progressFill;
    sf::Texture                 heartTexture;
    std::vector<sf::Sprite>     heartSprites;

    // Reward scoreboard (feature 2)
    // std::optional because the texture lives in GameWorld, built after Game
    std::optional<sf::Sprite>   rewardIcon;
    sf::Text                    rewardCountText;

    // ── PNG image buttons ─────────────────────────────────────────────────
    // pause.png / play.png  and  mute.png / unmute.png
    // All four textures loaded once; active sprite swapped via optional.
    sf::Texture                 texPause;
    sf::Texture                 texPlay;
    sf::Texture                 texMute;
    sf::Texture                 texUnmute;

    std::optional<sf::Sprite>   pauseBtn;   // shows pause.png or play.png
    std::optional<sf::Sprite>   muteBtn;    // shows mute.png  or unmute.png

    sf::FloatRect               pauseHitbox;
    sf::FloatRect               muteHitbox;

    static constexpr float kBtnSize = 36.f;   // both buttons rendered at 36×36 px

    // ── Background ────────────────────────────────────────────────────────
    sf::Texture                 menuBgTexture;
    std::optional<sf::Sprite>   menuBgSprite;   // built once texture has loaded

    // ── Music (feature 1) ─────────────────────────────────────────────────
    // sf::Music is non-copyable and non-movable in SFML 3 → unique_ptr
    std::unique_ptr<sf::Music>  menuMusic;
    std::unique_ptr<sf::Music>  gameMusic;
    bool                        musicMuted;

    // ── Sound effects ─────────────────────────────────────────────────────
    // sf::Sound has no default constructor in SFML 3, so we store the buffer
    // separately and construct the sound into a std::optional once loaded.
    sf::SoundBuffer             hitSoundBuf;
    sf::SoundBuffer             collectSoundBuf;
    std::optional<sf::Sound>    hitSound;
    std::optional<sf::Sound>    collectSound;
    int                         prevRewardsCollected;  // detect new collections

    // ── Timers ────────────────────────────────────────────────────────────
    float       timeLeft;
    float       freezeTimer;
    static constexpr float kBaseDuration   = 60.f;
    static constexpr float kDeltaDuration  =  5.f;
    static constexpr float kFreezeDuration = 5.f;

    // ── Private helpers ───────────────────────────────────────────────────
    void initHUD();
    void updateHUD();
    void renderHUD();
    void resetWorld();

    void startMenuMusic();
    void startGameMusic();
    void stopAllMusic();
    void applyMuteState();
    void swapPauseTexture(bool paused);  // immediately swaps pause/play PNG

public:
    Game(sf::Vector2u windowSize);
    ~Game();

    sf::RenderWindow& getWindow();
    void handleEvents();
    void update(float dt);
    void render();
};

#endif
