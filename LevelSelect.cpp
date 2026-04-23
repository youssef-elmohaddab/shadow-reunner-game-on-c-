#include "LevelSelect.hpp"
#include <iostream>
#include <string>

LevelSelect::LevelSelect(sf::RenderWindow& window, int unlockedLevels_)
    : unlockedLevels(unlockedLevels_), selected(0),
      titleText(font, "Select Level", 52u),backText(font, "Back", 32u)
{
    if (!font.openFromFile("data/fonts/arial.ttf"))
        std::cout << "Warning: could not load LevelSelect font.\n";
    build(window);
}

void LevelSelect::setUnlockedLevels(int n)
{
    unlockedLevels = n;
    updateColors();
}

void LevelSelect::build(sf::RenderWindow& window)
{
    buttons.clear();
    labels.clear();

    const float sw = static_cast<float>(window.getSize().x);
    const float sh = static_cast<float>(window.getSize().y);

    // Title
    titleText = sf::Text(font, "Select Level", 52u);
    titleText.setFillColor(sf::Color::White);
    auto tb = titleText.getLocalBounds();
    titleText.setOrigin({tb.size.x / 2.f, 0.f});
    titleText.setPosition({sw / 2.f, sh * 0.08f});

    // 10 level buttons in 2 rows of 5
    const int   cols  = 5;
    const float btnW  = 110.f;
    const float btnH  = 100.f;
    const float gapX  = 24.f;
    const float gapY  = 20.f;
    float totalW      = cols * btnW + (cols - 1) * gapX;
    float startX      = (sw - totalW) / 2.f;
    float row0Y       = sh * 0.28f;
    float row1Y       = row0Y + btnH + gapY;

    for (int i = 0; i < kTotalLevels; ++i)
    {
        int   col  = i % cols;
        int   row  = i / cols;
        float btnX = startX + col * (btnW + gapX);
        float btnY = (row == 0) ? row0Y : row1Y;

        sf::RectangleShape btn({btnW, btnH});
        btn.setPosition({btnX, btnY});
        btn.setOutlineThickness(2.f);
        buttons.push_back(btn);

        sf::Text txt(font, "Level\n" + std::to_string(i + 1), 20u);
        auto lb = txt.getLocalBounds();
        txt.setOrigin({lb.size.x / 2.f, lb.size.y / 2.f});
        txt.setPosition({btnX + btnW / 2.f, btnY + btnH / 2.f});
        labels.push_back(txt);
    }

    // Back button — below both rows
    float backY = row1Y + btnH + gapY * 2.f;
    backBtn.setSize({160.f, 55.f});
    backBtn.setPosition({sw / 2.f - 80.f, backY});
    backBtn.setOutlineThickness(2.f);

    // backText = sf::Text(font, "Back", 32u);
    auto bb = backText.getLocalBounds();
    backText.setOrigin({bb.size.x / 2.f, bb.size.y / 2.f});
    backText.setPosition({sw / 2.f, backY + 27.f});

    updateColors();
}

void LevelSelect::updateColors()
{
    for (int i = 0; i < kTotalLevels; ++i)
    {
        bool locked = (i >= unlockedLevels);
        bool isSel  = (selected == i);

        if (locked)
        {
            buttons[i].setFillColor(sf::Color(40, 40, 40));
            buttons[i].setOutlineColor(sf::Color(80, 80, 80));
            labels[i].setFillColor(sf::Color(90, 90, 90));
        }
        else if (isSel)
        {
            buttons[i].setFillColor(sf::Color(80, 160, 255));
            buttons[i].setOutlineColor(sf::Color::White);
            labels[i].setFillColor(sf::Color::White);
        }
        else
        {
            buttons[i].setFillColor(sf::Color(50, 50, 120));
            buttons[i].setOutlineColor(sf::Color(140, 140, 200));
            labels[i].setFillColor(sf::Color(220, 220, 255));
        }
    }

    bool backSel = (selected == kTotalLevels);
    backBtn.setFillColor(backSel ? sf::Color(100, 100, 255) : sf::Color(50, 50, 50));
    backBtn.setOutlineColor(backSel ? sf::Color::White : sf::Color(150, 150, 150));
    backText.setFillColor(sf::Color::White);
}

void LevelSelect::draw(sf::RenderWindow& window)
{
    window.draw(titleText);
    for (int i = 0; i < kTotalLevels; ++i)
    {
        window.draw(buttons[i]);
        window.draw(labels[i]);
    }
    window.draw(backBtn);
    window.draw(backText);
}

int LevelSelect::handleEvent(const sf::Event& event, sf::RenderWindow& window)
{
    // Keyboard navigation
    if (const auto* key = event.getIf<sf::Event::KeyPressed>())
    {
        if (key->scancode == sf::Keyboard::Scancode::Left)
        {
            if (selected > 0)
            {
                --selected;
                updateColors();
            }
        }
        else if (key->scancode == sf::Keyboard::Scancode::Right)
        {
            if (selected < kTotalLevels)
            {
                ++selected;
                updateColors();
            }
        }
        else if (key->scancode == sf::Keyboard::Scancode::Enter ||
                 key->scancode == sf::Keyboard::Scancode::Space)
        {
            if (selected == kTotalLevels) return -2;           // back
            if (selected < unlockedLevels) return selected;    // play
        }
        else if (key->scancode == sf::Keyboard::Scancode::Escape)
            return -2;
    }

    // Mouse hover
    if (const auto* moved = event.getIf<sf::Event::MouseMoved>())
    {
        sf::Vector2f mouse = window.mapPixelToCoords(moved->position);
        for (int i = 0; i < kTotalLevels; ++i)
            if (buttons[i].getGlobalBounds().contains(mouse) && i < unlockedLevels)
            {
                selected = i;
                updateColors();
            }
        if (backBtn.getGlobalBounds().contains(mouse))
        {
            selected = kTotalLevels;
            updateColors();
        }
    }

    // Mouse click
    if (const auto* click = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (click->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mouse = window.mapPixelToCoords(click->position);
            for (int i = 0; i < kTotalLevels; ++i)
                if (buttons[i].getGlobalBounds().contains(mouse) && i < unlockedLevels)
                    return i;
            if (backBtn.getGlobalBounds().contains(mouse))
                return -2;
        }
    }

    return -1;
}
