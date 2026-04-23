#ifndef LevelSelect_h
#define LevelSelect_h

#include <SFML/Graphics.hpp>
#include <vector>

class LevelSelect
{
public:
    LevelSelect(sf::RenderWindow& window, int unlockedLevels);

    void draw(sf::RenderWindow& window);

    int  handleEvent(const sf::Event& event, sf::RenderWindow& window);
    void setUnlockedLevels(int n);

private:
    sf::Font                        font;
    int                             unlockedLevels;
    static constexpr int            kTotalLevels = 10;
    int                             selected;

    std::vector<sf::RectangleShape> buttons;
    std::vector<sf::Text>           labels;
    sf::RectangleShape              backBtn;
    sf::Text                        backText;
    sf::Text                        titleText;

    void build(sf::RenderWindow& window);
    void updateColors();
};

#endif
