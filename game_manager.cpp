#include "game_manager.hpp"

#include "player.hpp"
#include "bullet.hpp"

#include <SFML/Graphics.hpp>

#include <iostream>

GameManager::GameManager() : 
    window([] () -> sf::RenderWindow {
        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
        auto screenWidth = desktop.width * 3 / 4;
        auto screenHeight = desktop.height * 3 / 4;
        return {sf::VideoMode(screenWidth, screenHeight, desktop.bitsPerPixel), "Asteroids"};
    }())
{
    window.setFramerateLimit(60);

    addObject<Player>(window);
    //addObject<BulletManager>(window, max_bullets);
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
