#include <SFML/Graphics.hpp>
#include "game.hpp"
#include <iostream>
#include <stdexcept>

int main()
{
    try
    {
        Game game({1200u, 800u});
        sf::Clock clock;

        while (game.getWindow().isOpen())
        {
            float dt = clock.restart().asSeconds();
            if (dt > 0.05f) dt = 0.05f;   // cap to avoid spiral-of-death

            game.handleEvents();
            game.update(dt);
            game.render();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::cerr << "Fatal error: unknown exception\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
