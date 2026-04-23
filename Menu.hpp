#ifndef Menu_h
#define Menu_h

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Menu
{
public:
    enum class Choice { Play, About, Quit, None };

    Menu(sf::RenderWindow& window);

    void   draw(sf::RenderWindow& window);
    Choice handleEvent(const sf::Event& event,sf::RenderWindow& window);



private:
    sf::Font                font;
    std::vector<sf::Text>   items;
    int                     selected;

    //  std::vector<sf::Text> items;
    std::vector<sf::RectangleShape> buttons;
    //  int selected;

    void updateColors();
};

#endif
