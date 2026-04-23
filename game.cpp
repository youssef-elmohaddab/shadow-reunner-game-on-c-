#include "game.hpp"
#include <optional>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>

// Fonctions auxiliaires pour la musique

void Game::startMenuMusic()
{
    // Arrête la musique du jeu si elle joue
    if (gameMusic) gameMusic->stop();

    try {
        menuMusic = std::make_unique<sf::Music>();
        if (!menuMusic->openFromFile("data/audio/menu_music.ogg"))
            throw std::runtime_error("Could not open menu_music.ogg");
        menuMusic->setLooping(true);
        menuMusic->setVolume(musicMuted ? 0.f : 60.f);
        menuMusic->play();
    } catch (const std::exception& e) {
        std::cerr << "Warning: " << e.what() << "\n";
        menuMusic.reset();   // laisse nul — musique simplement absente
    }
}

void Game::startGameMusic()
{
    if (menuMusic) menuMusic->stop();

    try {
        gameMusic = std::make_unique<sf::Music>();
        if (!gameMusic->openFromFile("data/audio/game_music.ogg"))
            throw std::runtime_error("Could not open game_music.ogg");
        gameMusic->setLooping(true);
        gameMusic->setVolume(musicMuted ? 0.f : 60.f);
        gameMusic->play();
    } catch (const std::exception& e) {
        std::cerr << "Warning: " << e.what() << "\n";
        gameMusic.reset();
    }
}

void Game::stopAllMusic()
{
    if (menuMusic) menuMusic->stop();
    if (gameMusic) gameMusic->stop();
}

void Game::swapPauseTexture(bool paused)
{
    sf::Texture& t = paused ? texPlay : texPause;
    float tw = static_cast<float>(t.getSize().x);
    float th = static_cast<float>(t.getSize().y);
    if (tw == 0.f || th == 0.f) return;

    // Préserve la position avant reconstruction
    sf::Vector2f pos = pauseBtn ? pauseBtn->getPosition() : sf::Vector2f{0.f, 0.f};
    pauseBtn.emplace(t);
    pauseBtn->setScale({kBtnSize / tw, kBtnSize / th});
    pauseBtn->setPosition(pos);
    pauseHitbox = pauseBtn->getGlobalBounds();
}

void Game::applyMuteState()
{
    float vol = musicMuted ? 0.f : 60.f;
    if (menuMusic) menuMusic->setVolume(vol);
    if (gameMusic) gameMusic->setVolume(vol);

    // Coupe/rétablit aussi les effets sonores
    float sfxVol = musicMuted ? 0.f : 100.f;
    if (hitSound)     hitSound->setVolume(sfxVol);
    if (collectSound) collectSound->setVolume(sfxVol);

    sf::Texture& t = musicMuted ? texMute : texUnmute;
    float tw = static_cast<float>(t.getSize().x);
    float th = static_cast<float>(t.getSize().y);
    if (tw == 0.f || th == 0.f) return;

    sf::Vector2f pos = muteBtn ? muteBtn->getPosition() : sf::Vector2f{0.f, 0.f};
    muteBtn.emplace(t);
    muteBtn->setScale({kBtnSize / tw, kBtnSize / th});
    muteBtn->setPosition(pos);
    muteHitbox = muteBtn->getGlobalBounds();
}

// Constructeur

Game::Game(sf::Vector2u windowSize)
    : window(sf::VideoMode({windowSize.x, windowSize.y}), "Shadow Runner"),
      camera(sf::FloatRect({0.f, 0.f},
                            {static_cast<float>(windowSize.x),
                             static_cast<float>(windowSize.y)})),
      state(State::Menu),
      menu(window),
      levelSelect(window, 1),
      world(nullptr),
      currentLevel(0),
      unlockedLevels(1),
      musicMuted(false),
      prevRewardsCollected(0),
      timeLeft(kBaseDuration),
      freezeTimer(0.f),

      timerText(uiFont, "", 26u),
      rewardCountText(uiFont, "x0", 22u)

{
    window.setVerticalSyncEnabled(true);

    // Police — gérée avec exceptions
    try {
        if (!uiFont.openFromFile("data/fonts/arial.ttf"))
            throw std::runtime_error("Could not load UI font");
    } catch (const std::exception& e) {
        std::cerr << "Warning: " << e.what() << "\n";
    }

    // Réinitialise les textes après chargement de la police
    timerText       = sf::Text(uiFont, "", 26u);
    rewardCountText = sf::Text(uiFont, "x0", 22u);

    // Texture du cœur
    try {
        if (!heartTexture.loadFromFile("data/images/heart.png"))
            throw std::runtime_error("Could not load heart texture");
    } catch (const std::exception& e) {
        std::cerr << "Warning: " << e.what() << "\n";
    }

    // Arrière-plan du menu
    try {
        if (!menuBgTexture.loadFromFile("data/images/menuBackground.png"))
            throw std::runtime_error("Could not load menu background");
        // Construit dans l'optional maintenant que la texture est prête
        menuBgSprite.emplace(menuBgTexture);
        menuBgSprite->setScale({
            static_cast<float>(window.getSize().x) / menuBgTexture.getSize().x,
            static_cast<float>(window.getSize().y) / menuBgTexture.getSize().y
        });
    } catch (const std::exception& e) {
        std::cerr << "Warning: " << e.what() << "\n";
    }

    // Charge le buffer d'abord, puis construit le son
    try {
        if (!hitSoundBuf.loadFromFile("data/audio/hit.wav"))
            throw std::runtime_error("Could not load hit.wav");
        hitSound.emplace(hitSoundBuf);
        if (musicMuted) hitSound->setVolume(0.f);
    } catch (const std::exception& e) {
        std::cerr << "Warning: " << e.what() << "\n";
    }

    try {
        if (!collectSoundBuf.loadFromFile("data/audio/collect.flac"))
            throw std::runtime_error("Could not load collect.flac");
        collectSound.emplace(collectSoundBuf);
        if (musicMuted) collectSound->setVolume(0.f);
    } catch (const std::exception& e) {
        std::cerr << "Warning: " << e.what() << "\n";
    }

    resetWorld();
    initHUD();
    startMenuMusic();
}

Game::~Game()
{
    stopAllMusic();
    delete world;
}

void Game::resetWorld()
{
    delete world;
    try {
        world = new GameWorld(window, currentLevel);
    } catch (const std::exception& e) {
        std::cerr << "Fatal: failed to create GameWorld — " << e.what() << "\n";
        window.close();
        return;
    }

    timeLeft             = kBaseDuration - kDeltaDuration * static_cast<float>(currentLevel);
    freezeTimer          = 0.f;
    prevRewardsCollected = 0;

    float sw = static_cast<float>(window.getSize().x);
    float sh = static_cast<float>(window.getSize().y);
    camera.setCenter({sw / 2.f, sh / 2.f});
}

void Game::initHUD()
{
    const float sw = static_cast<float>(window.getSize().x);

    // Minuteur
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition({16.f, 10.f});

    // Barre de progression
    progressBg.setSize({160.f, 10.f});
    progressBg.setPosition({16.f, 44.f});
    progressBg.setFillColor(sf::Color(55, 55, 55));
    progressBg.setOutlineThickness(1.f);
    progressBg.setOutlineColor(sf::Color(160, 160, 160));

    progressFill.setSize({0.f, 10.f});
    progressFill.setPosition({16.f, 44.f});
    progressFill.setFillColor(sf::Color(70, 200, 70));

    // Cœurs
    heartSprites.clear();
    float srcW = static_cast<float>(heartTexture.getSize().x);
    float srcH = static_cast<float>(heartTexture.getSize().y);
    for (int i = 0; i < 3; ++i)
    {
        sf::Sprite spr(heartTexture);
        if (srcW > 0 && srcH > 0)
            spr.setScale({28.f / srcW, 28.f / srcH});
        spr.setPosition({sw - 35.f - (2 - i) * 34.f, 10.f});
        heartSprites.push_back(spr);
    }

    if (world)
    {
        const sf::Texture& rewTex = world->getRewardTexture();
        rewardIcon.emplace(rewTex);
        float rW = static_cast<float>(rewTex.getSize().x);
        float rH = static_cast<float>(rewTex.getSize().y);
        if (rW > 0 && rH > 0)
            rewardIcon->setScale({32.f / rW, 32.f / rH});
        rewardIcon->setPosition({16.f, 60.f});
    }
    rewardCountText.setFillColor(sf::Color(255, 220, 60));
    rewardCountText.setPosition({54.f, 63.f});

    auto loadBtnTex = [](sf::Texture& tex, const std::string& path)
    {
        try {
            if (!tex.loadFromFile(path))
                throw std::runtime_error("Cannot load " + path);
            tex.setSmooth(true);
        } catch (const std::exception& e) {
            std::cerr << "Warning: " << e.what() << "\n";
        }
    };
    loadBtnTex(texPause,   "data/images/pause.png");
    loadBtnTex(texPlay,    "data/images/play.png");
    loadBtnTex(texMute,    "data/images/mute.png");
    loadBtnTex(texUnmute,  "data/images/unmute.png");

    // Auxiliaire: crée un sprite mis à l'échelle
    auto makeBtn = [&](sf::Texture& tex) -> sf::Sprite
    {
        sf::Sprite spr(tex);
        float tw = static_cast<float>(tex.getSize().x);
        float th = static_cast<float>(tex.getSize().y);
        if (tw > 0.f && th > 0.f)
            spr.setScale({kBtnSize / tw, kBtnSize / th});
        return spr;
    };

    // Position: bouton pause à gauche de mute, aligné à droite
    const float btnY  = 46.f;
    const float gap   = 6.f;
    const float muteX  = sw - 7.f  - kBtnSize;
    const float pauseX = muteX - gap - kBtnSize;

    // Crée les sprites initiaux
    pauseBtn.emplace(makeBtn(state == State::Paused ? texPlay : texPause));
    pauseBtn->setPosition({pauseX, btnY});

    muteBtn.emplace(makeBtn(musicMuted ? texMute : texUnmute));
    muteBtn->setPosition({muteX, btnY});

    pauseHitbox = pauseBtn->getGlobalBounds();
    muteHitbox  = muteBtn->getGlobalBounds();
}

void Game::updateHUD()
{
    // Minuteur
    std::ostringstream ss;
    ss << "Time: " << static_cast<int>(timeLeft) << "s";
    timerText.setString(ss.str());

    const float Xstart  = 100.f;
    const float Xbunker = 10000.f + 555 * currentLevel;
    const float Xplayer = world->getPlayer().getPosition().x;
    float progress = std::clamp((Xplayer - Xstart) / (Xbunker - Xstart), 0.f, 1.f);
    progressFill.setSize({160.f * progress, 10.f});

    if      (progress < 0.5f)  progressFill.setFillColor(sf::Color(70,  200,  70));
    else if (progress < 0.75f) progressFill.setFillColor(sf::Color(220, 190,  40));
    else                       progressFill.setFillColor(sf::Color(80,  160, 255));

    // Cœurs
    int lives = world->getPlayer().getLives();
    for (int i = 0; i < 3; ++i)
        heartSprites[i].setColor(i < lives
                                 ? sf::Color(255, 255, 255, 255)
                                 : sf::Color(60,  60,  60,  140));

    rewardCountText.setString("x" + std::to_string(world->getRewardsCollected()));

    // Change l'image du bouton mute
    {
        sf::Texture& t = musicMuted ? texMute : texUnmute;
        float tw = static_cast<float>(t.getSize().x);
        float th = static_cast<float>(t.getSize().y);
        if (tw > 0.f && th > 0.f && muteBtn)
        {
            sf::Vector2f pos = muteBtn->getPosition();
            muteBtn.emplace(t);
            muteBtn->setScale({kBtnSize / tw, kBtnSize / th});
            muteBtn->setPosition(pos);
            muteHitbox = muteBtn->getGlobalBounds();
        }
    }
}

void Game::renderHUD()
{
    window.setView(window.getDefaultView());

    window.draw(timerText);
    window.draw(progressBg);
    window.draw(progressFill);
    for (auto& h : heartSprites) window.draw(h);

    if (rewardIcon.has_value()) window.draw(*rewardIcon);
    window.draw(rewardCountText);

    if (pauseBtn) window.draw(*pauseBtn);
    if (muteBtn)  window.draw(*muteBtn);

    // Indicateur de niveau en haut au centre
    sf::Text lvlTxt(uiFont, "Level " + std::to_string(currentLevel + 1), 24u);
    lvlTxt.setFillColor(sf::Color(200, 200, 255));
    auto lb = lvlTxt.getLocalBounds();
    lvlTxt.setOrigin({lb.size.x / 2.f, 0.f});
    lvlTxt.setPosition({window.getSize().x / 2.f, 10.f});
    window.draw(lvlTxt);
}

sf::RenderWindow& Game::getWindow() { return window; }

void Game::handleEvents()
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>()) window.close();

        if (const auto* key = event->getIf<sf::Event::KeyPressed>())
            if (key->scancode == sf::Keyboard::Scancode::M)
            {
                musicMuted = !musicMuted;
                applyMuteState();
            }

        if (const auto* click = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (click->button == sf::Mouse::Button::Left)
            {
                sf::Vector2f mouse = window.mapPixelToCoords(click->position);
                if (muteHitbox.contains(mouse))
                {
                    musicMuted = !musicMuted;
                    applyMuteState();
                }
            }
        }

        if (state == State::Menu)
        {
            Menu::Choice choice = menu.handleEvent(*event, window);
            if      (choice == Menu::Choice::Play)  { state = State::LevelSelect; }
            else if (choice == Menu::Choice::About) { state = State::About; }
            else if (choice == Menu::Choice::Quit)  { window.close(); }
        }
        else if (state == State::LevelSelect)
        {
            int chosen = levelSelect.handleEvent(*event, window);
            if (chosen == -2)
            {
                state = State::Menu;
            }
            else if (chosen >= 0)
            {
                currentLevel = chosen;
                resetWorld();
                initHUD();
                startGameMusic();
                state = State::Playing;
            }
        }
        else if (state == State::Playing)
        {
            if (const auto* key = event->getIf<sf::Event::KeyPressed>())
            {
                if (key->scancode == sf::Keyboard::Scancode::Escape)
                {
                    stopAllMusic();
                    state = State::Menu;
                    startMenuMusic();
                }
                else if (key->scancode == sf::Keyboard::Scancode::P)
                {
                    state = State::Paused;
                    if (gameMusic) gameMusic->pause();
                    swapPauseTexture(true);
                }
                else if (key->scancode == sf::Keyboard::Scancode::F1)
                    world->debugHitboxes = !world->debugHitboxes;
                else if (key->scancode == sf::Keyboard::Scancode::Up   ||
                         key->scancode == sf::Keyboard::Scancode::Space ||
                         key->scancode == sf::Keyboard::Scancode::W)
                    world->getPlayer().jump();
            }
            if (const auto* key = event->getIf<sf::Event::KeyReleased>())
                if (key->scancode == sf::Keyboard::Scancode::S ||
                    key->scancode == sf::Keyboard::Scancode::Down)
                    world->getPlayer().standUp();

            // Clic sur bouton pause
            if (const auto* click = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (click->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2f mouse = window.mapPixelToCoords(click->position);
                    if (pauseHitbox.contains(mouse))
                    {
                        state = State::Paused;
                        if (gameMusic) gameMusic->pause();
                        swapPauseTexture(true);
                    }
                }
            }
        }
        else if (state == State::Paused)
        {
            // Reprendre avec P, Espace ou Entrée
            if (const auto* key = event->getIf<sf::Event::KeyPressed>())
            {
                if (key->scancode == sf::Keyboard::Scancode::P     ||
                    key->scancode == sf::Keyboard::Scancode::Space  ||
                    key->scancode == sf::Keyboard::Scancode::Enter)
                {
                    state = State::Playing;
                    if (!musicMuted && gameMusic) gameMusic->play();
                    swapPauseTexture(false);
                }
                else if (key->scancode == sf::Keyboard::Scancode::Escape)
                {
                    stopAllMusic();
                    state = State::Menu;
                    startMenuMusic();
                }
            }
            // Clic sur bouton reprise
            if (const auto* click = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (click->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2f mouse = window.mapPixelToCoords(click->position);
                    if (pauseHitbox.contains(mouse))
                    {
                        state = State::Playing;
                        if (!musicMuted && gameMusic) gameMusic->play();
                        swapPauseTexture(false);
                    }
                }
            }
        }
        else if (state == State::Frozen)
        {
            // Aucune entrée pendant le gel
        }
        else if (state == State::Won)
        {
            if (const auto* key = event->getIf<sf::Event::KeyPressed>())
            {
                if (key->scancode == sf::Keyboard::Scancode::Enter ||
                    key->scancode == sf::Keyboard::Scancode::Space)
                {
                    if (currentLevel + 1 < kTotalLevels)
                    {
                        currentLevel++;
                        if (currentLevel + 1 > unlockedLevels)
                            unlockedLevels = currentLevel + 1;
                        levelSelect.setUnlockedLevels(unlockedLevels);
                        resetWorld();
                        initHUD();
                        startGameMusic();
                        state = State::Playing;
                    }
                    else
                    {
                        stopAllMusic();
                        state = State::Menu;
                        startMenuMusic();
                    }
                }
                else if (key->scancode == sf::Keyboard::Scancode::Escape)
                {
                    stopAllMusic();
                    state = State::Menu;
                    startMenuMusic();
                }
            }
        }
        else if (state == State::About || state == State::Over)
        {
            if (const auto* key = event->getIf<sf::Event::KeyPressed>())
                if (key->scancode == sf::Keyboard::Scancode::Escape ||
                    key->scancode == sf::Keyboard::Scancode::Enter)
                {
                    if (state == State::Over) { stopAllMusic(); startMenuMusic(); }
                    state = State::Menu;
                }
        }
    }
}

void Game::update(float dt)
{
    // En pause — ne fait rien
    if (state == State::Paused) return;

    // Gelé
    if (state == State::Frozen)
    {
        freezeTimer -= dt;
        if (freezeTimer <= 0.f)
            state = State::Playing;
        return;
    }

    if (state != State::Playing) return;

    // Compte à rebours
    timeLeft -= dt;
    if (timeLeft <= 0.f)
    {
        timeLeft = 0.f;
        stopAllMusic();
        startMenuMusic();
        state = State::Over;
        return;
    }

    // Entrées de mouvement
    bool ctrl  = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LControl) ||
                 sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::RControl);
    bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right) ||
                 sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D);
    bool left  = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left)  ||
                 sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A);
    bool down  = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)     ||
                 sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down);

    if (down) world->getPlayer().crouch();

    if (right)
    {
        if (ctrl) world->getPlayer().moveRunForward();
        else      world->getPlayer().moveWalkForward();
    }
    else if (left)
    {
        if (ctrl) world->getPlayer().moveRunBack();
        else      world->getPlayer().moveWalkBack();
    }
    else
    {
        if (!world->getPlayer().getIsCrouching())
            world->getPlayer().setIdle();
    }

    // Mise à jour du monde (obstacles polymorphiques)
    GameWorld::HitResult hit = world->update(dt);

    if (hit == GameWorld::HitResult::Dead)
    {
        if (hitSound) hitSound->play();
        stopAllMusic();
        startMenuMusic();
        state = State::Over;
        return;
    }
    else if (hit == GameWorld::HitResult::Hurt)
    {
        if (hitSound) hitSound->play();
        freezeTimer = kFreezeDuration;
        state       = State::Frozen;
        return;
    }
    else if (hit == GameWorld::HitResult::Won)
    {
        state = State::Won;
        return;
    }

    int nowCollected = world->getRewardsCollected();
    if (nowCollected > prevRewardsCollected)
    {
        if (collectSound) collectSound->play();
        prevRewardsCollected = nowCollected;
    }

    // Suivi de la caméra
    float sw      = static_cast<float>(window.getSize().x);
    float sh      = static_cast<float>(window.getSize().y);
    float playerX = world->getPlayer().getPosition().x;
    float camLeft = camera.getCenter().x - sw / 2.f;
    float scrX    = playerX - camLeft;

    if      (scrX > sw * 0.50f) camera.move({scrX - sw * 0.50f, 0.f});
    else if (scrX < sw * 0.10f) camera.move({scrX - sw * 0.10f, 0.f});

    if (camera.getCenter().x < sw / 2.f)
        camera.setCenter({sw / 2.f, sh / 2.f});

    updateHUD();
}

void Game::render()
{
    window.clear(sf::Color(30, 30, 30));

    if (state == State::Menu)
    {
        window.setView(window.getDefaultView());
        if (menuBgSprite.has_value()) window.draw(*menuBgSprite);
        // Bouton muet visible dans le menu
        if (muteBtn) window.draw(*muteBtn);
        menu.draw(window);
    }
    else if (state == State::LevelSelect)
    {
        window.setView(window.getDefaultView());
        if (menuBgSprite.has_value()) window.draw(*menuBgSprite);
        if (muteBtn) window.draw(*muteBtn);
        levelSelect.draw(window);
    }
    else if (state == State::Playing || state == State::Frozen || state == State::Paused)
    {
        window.setView(camera);
        world->render(window);
        renderHUD();

        if (state == State::Frozen)
        {
            window.setView(window.getDefaultView());

            sf::RectangleShape overlay;
            overlay.setSize(sf::Vector2f(window.getSize()));
            overlay.setFillColor(sf::Color(0, 0, 0, 130));
            window.draw(overlay);

            std::ostringstream ss;
            ss << "Ouch!  Resuming in " << (static_cast<int>(freezeTimer) + 1) << "...";
            sf::Text msg(uiFont, ss.str(), 38u);
            msg.setFillColor(sf::Color(255, 80, 80));
            auto lb = msg.getLocalBounds();
            msg.setOrigin({lb.size.x / 2.f, lb.size.y / 2.f});
            msg.setPosition({window.getSize().x / 2.f, window.getSize().y / 2.f});
            window.draw(msg);
        }
        else if (state == State::Paused)
        {
            // Superposition de pause
            window.setView(window.getDefaultView());

            sf::RectangleShape overlay;
            overlay.setSize(sf::Vector2f(window.getSize()));
            overlay.setFillColor(sf::Color(0, 0, 0, 150));
            window.draw(overlay);

            sf::Text pauseTxt(uiFont,
                "PAUSED\n\nPress P / Enter to resume\nEsc to return to menu",
                38u);
            pauseTxt.setFillColor(sf::Color(180, 220, 255));
            auto lb = pauseTxt.getLocalBounds();
            pauseTxt.setOrigin({lb.size.x / 2.f, lb.size.y / 2.f});
            pauseTxt.setPosition({window.getSize().x / 2.f, window.getSize().y / 2.f});
            window.draw(pauseTxt);

            // Affiche les deux boutons sur l'écran de pause aussi
            if (pauseBtn) window.draw(*pauseBtn);
            if (muteBtn)  window.draw(*muteBtn);
        }
    }
    else if (state == State::Won)
    {
        window.setView(window.getDefaultView());

        sf::RectangleShape overlay;
        overlay.setSize(sf::Vector2f(window.getSize()));
        overlay.setFillColor(sf::Color(0, 0, 0, 160));
        window.draw(overlay);

        bool hasNext = (currentLevel + 1 < kTotalLevels);
        std::string msg = hasNext
            ? "You reached the bunker!\n\nLevel " + std::to_string(currentLevel + 1)
              + " complete!\n\nPress Enter for Level "
              + std::to_string(currentLevel + 2)
              + "\nor Esc to return to menu."
            : "You completed all levels!\n\nCongratulations!\n\nPress Enter or Esc.";

        sf::Text wonTxt(uiFont, msg, 36u);
        wonTxt.setFillColor(sf::Color(80, 255, 120));
        auto lb = wonTxt.getLocalBounds();
        wonTxt.setOrigin({lb.size.x / 2.f, lb.size.y / 2.f});
        wonTxt.setPosition({window.getSize().x / 2.f, window.getSize().y / 2.f});
        window.draw(wonTxt);
    }
    else if (state == State::About)
    {
        window.setView(window.getDefaultView());
        sf::Text about(uiFont,
            "Run to the bunker before time runs out!\n\n"
            "Arrow / WASD        Move\n"
            "Ctrl + Arrow        Run\n"
            "Up / Space / W      Jump\n"
            "Down / S            Crouch  (release to stand)\n"
            "P                   Pause / Resume\n"
            "M                   Toggle music\n"
            "Esc                 Back to menu\n\n"
            "You have 3 lives.\n"
            "Hitting an obstacle freezes the screen for 5 s\n"
            "then pushes you back 100 px.\n\n"
            "Collect rewards to increase your score!\n\n"
            "Reach the bunker to complete the level!\n\n"
            "Press Enter or Esc to go back.", 26u);
        about.setFillColor(sf::Color::White);
        about.setPosition({80.f, 60.f});
        window.draw(about);
    }
    else if (state == State::Over)
    {
        window.setView(window.getDefaultView());
        sf::Text over(uiFont,
            "Game Over!\n\nPress Enter to return to menu.", 40u);
        over.setFillColor(sf::Color::Red);
        auto lb = over.getLocalBounds();
        over.setOrigin({lb.size.x / 2.f, lb.size.y / 2.f});
        over.setPosition({window.getSize().x / 2.f, window.getSize().y / 2.f});
        window.draw(over);
    }

    window.display();
}
