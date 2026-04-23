#ifndef Game_h
#define Game_h

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <optional>
#include <memory>
#include "Menu.hpp"
#include "GameWorld.hpp"
#include "LevelSelect.hpp"

// Classe principale du jeu gérant les états et la boucle de jeu
class Game
{
public:
    // États possibles du jeu
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

    // Interface utilisateur (HUD)
    sf::Text                    timerText;
    sf::RectangleShape          progressBg;
    sf::RectangleShape          progressFill;
    sf::Texture                 heartTexture;
    std::vector<sf::Sprite>     heartSprites;

    // Compteur de récompenses collectées
    std::optional<sf::Sprite>   rewardIcon;
    sf::Text                    rewardCountText;

    // Textures des boutons pause/play et mute/unmute
    sf::Texture                 texPause;
    sf::Texture                 texPlay;
    sf::Texture                 texMute;
    sf::Texture                 texUnmute;

    std::optional<sf::Sprite>   pauseBtn;
    std::optional<sf::Sprite>   muteBtn;

    sf::FloatRect               pauseHitbox;
    sf::FloatRect               muteHitbox;

    static constexpr float kBtnSize = 36.f;

    // Arrière-plan du menu
    sf::Texture                 menuBgTexture;
    std::optional<sf::Sprite>   menuBgSprite;

    // Musique de fond (menu et jeu)
    std::unique_ptr<sf::Music>  menuMusic;
    std::unique_ptr<sf::Music>  gameMusic;
    bool                        musicMuted;

    // Effets sonores
    sf::SoundBuffer             hitSoundBuf;
    sf::SoundBuffer             collectSoundBuf;
    std::optional<sf::Sound>    hitSound;
    std::optional<sf::Sound>    collectSound;
    int                         prevRewardsCollected;

    // Timers pour le jeu
    float       timeLeft;
    float       freezeTimer;
    static constexpr float kBaseDuration   = 60.f;
    static constexpr float kDeltaDuration  =  5.f;
    static constexpr float kFreezeDuration = 5.f;

    // Fonctions privées d'initialisation et de mise à jour
    void initHUD();
    void updateHUD();
    void renderHUD();
    void resetWorld();

    void startMenuMusic();
    void startGameMusic();
    void stopAllMusic();
    void applyMuteState();
    void swapPauseTexture(bool paused);

public:
    Game(sf::Vector2u windowSize);
    ~Game();

    sf::RenderWindow& getWindow();
    void handleEvents();
    void update(float dt);
    void render();
};

#endif
