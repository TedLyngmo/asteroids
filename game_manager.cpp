#include "game_manager.hpp"

#include "bullet.hpp"
#include "player.hpp"
#include "rock.hpp"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <iostream>

GameManager::GameManager() : 
    window([] () -> sf::RenderWindow {
        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
        auto screenWidth = desktop.width * 4 / 5;
        auto screenHeight = desktop.height * 4 / 5;
        return {sf::VideoMode(screenWidth, screenHeight, desktop.bitsPerPixel), "Asteroids"};
    }())
{
    window.setFramerateLimit(60);

    addObject<Player>(window);
    addObject<RockManager>(window, 20);
}

void GameManager::run() {
    // Create delta time
    sf::Clock clock;
    duration time{};

    Player& player = *static_cast<Player*>(objects[0].get());

    sf::Event event;
    while (window.isOpen())
    {
        if (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            player.handleEvent(event);
        }

        //----
        for(auto& obj : objects) obj->Move(time);

        window.clear();

        for(auto& obj : objects) obj->Draw();

        window.display();
        //----
        time = clock.restart().asSeconds();
    }
}
