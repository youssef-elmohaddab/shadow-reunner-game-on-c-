#include "../include/Menu.hpp"
#include <iostream>

Menu::Menu(sf::RenderWindow& window) : selected(0)
{
    if (!font.openFromFile("data/fonts/arial.ttf"))
        std::cout << "Warning: could not load menu font.\n";

    const std::vector<std::string> labels = {"Play", "About", "Quit"};
    float startY = window.getSize().y / 2.f - 60.f;

    for (int i = 0; i < static_cast<int>(labels.size()); ++i)
    {
        sf::Text text(font, labels[i], 48u);
        sf::Vector2f textPos =
        {
            window.getSize().x / 2.f - 80.f,
            startY + i * 80.f
        };
        text.setPosition(textPos);
        items.push_back(text);

        sf::RectangleShape box({260.f, 65.f});
        box.setOrigin({0.f, 0.f});
        box.setPosition({textPos.x - 10.f, textPos.y - 5.f});
        box.setOutlineThickness(2.f);
        buttons.push_back(box);
    }

    updateColors();
}

void Menu::updateColors()
{
    for (int i = 0; i < static_cast<int>(items.size()); ++i)
    {
        if (i == selected)
        {
            buttons[i].setFillColor(sf::Color(100, 100, 255));
            buttons[i].setOutlineColor(sf::Color::White);
            items[i].setFillColor(sf::Color::White);
        }
        else
        {
            buttons[i].setFillColor(sf::Color(50, 50, 50));
            buttons[i].setOutlineColor(sf::Color(150, 150, 150));
            items[i].setFillColor(sf::Color(200, 200, 200));
        }
    }
}

void Menu::draw(sf::RenderWindow& window)
{
    for (auto& box  : buttons) window.draw(box);
    for (auto& text : items)   window.draw(text);
}

Menu::Choice Menu::handleEvent(const sf::Event& event, sf::RenderWindow& window)
{

    if (const auto* key = event.getIf<sf::Event::KeyPressed>())
    {
        if (key->scancode == sf::Keyboard::Scancode::Up)
        {
            selected = (selected - 1 + static_cast<int>(items.size()))
                       % static_cast<int>(items.size());
            updateColors();
        }
        else if (key->scancode == sf::Keyboard::Scancode::Down)
        {
            selected = (selected + 1) % static_cast<int>(items.size());
            updateColors();
        }
        else if (key->scancode == sf::Keyboard::Scancode::Enter ||
                 key->scancode == sf::Keyboard::Scancode::Space)
        {
            switch (selected)
            {
            case 0:
                return Choice::Play;
            case 1:
                return Choice::About;
            case 2:
                return Choice::Quit;
            }
        }
    }

    if (const auto* moved = event.getIf<sf::Event::MouseMoved>())
    {
        sf::Vector2f mouse = window.mapPixelToCoords(moved->position);
        for (int i = 0; i < static_cast<int>(buttons.size()); ++i)
        {
            if (buttons[i].getGlobalBounds().contains(mouse))
            {
                if (selected != i)
                {
                    selected = i;
                    updateColors();
                }
            }
        }
    }

    if (const auto* click = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (click->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mouse = window.mapPixelToCoords(click->position);
            for (int i = 0; i < static_cast<int>(buttons.size()); ++i)
            {
                if (buttons[i].getGlobalBounds().contains(mouse))
                {
                    switch (i)
                    {
                    case 0:
                        return Choice::Play;
                    case 1:
                        return Choice::About;
                    case 2:
                        return Choice::Quit;
                    }
                }
            }
        }
    }

    return Choice::None;
}
