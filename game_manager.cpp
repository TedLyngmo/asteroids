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
        auto screenWidth = desktop.width * 5 / 6;
        auto screenHeight = desktop.height * 5 / 6;
        return {sf::VideoMode(screenWidth, screenHeight, desktop.bitsPerPixel), "Asteroids"};
    }())
{
    window.setFramerateLimit(60);

    addObject<RockManager>(window, 20);
    auto bm = std::make_unique<BulletManager>(window);
    addObject<Player>(window, *bm.get());
    objects.push_back(std::move(bm));
}

void GameManager::run() {
    // Create delta time
    sf::Clock clock;
    duration time{};

    RockManager& rm = *static_cast<RockManager*>(objects[0].get());
    Player& player = *static_cast<Player*>(objects[1].get());
    BulletManager& bm = *static_cast<BulletManager*>(objects[2].get());

    sf::Event event;
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            //player.handleEvent(event);
        }

        //----
        for(auto& obj : objects) obj->Move(time);

        window.clear();

        for(auto& obj : objects) obj->Draw();

        window.display();

        for(auto& rock : rm) {
            bm.erase_if([&](const auto& bullet) {
                if(rock.isInside(bullet.getPosition())) {
                    rm.hit(rock);
                    return true;
                }
                return false;
            });
        }
        rm.update();
        //----
        time = clock.restart().asSeconds();
    }
}
