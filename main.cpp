#include <SFML/Graphics.hpp>
#include "game.hpp"
#include <iostream>
#include <stdexcept>

// Point d'entrée principal du jeu
int main()
{
    try
    {
        Game game({1200u, 800u});
        sf::Clock clock;

        // Boucle principale du jeu
        while (game.getWindow().isOpen())
        {
            float dt = clock.restart().asSeconds();
            if (dt > 0.05f) dt = 0.05f;

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
