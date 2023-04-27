#include "game_manager.hpp"

#include "memfont.hpp"
#include "bullet.hpp"
#include "player.hpp"
#include "rock.hpp"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <iostream>

GameManager::GameManager() : 
    window([] () -> sf::RenderWindow {
        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
        auto screenHeight = desktop.height * 5 / 6;
        auto screenWidth = screenHeight * 4 / 3;
        return {sf::VideoMode(screenWidth, screenHeight, desktop.bitsPerPixel), "Asteroids"};
    }()),
    view(window.getDefaultView())
{
    if(font.loadFromMemory(memfont.data(), memfont.size())==false) {
        std::cerr << "ERROR: Can't load font from memory." << std::endl;
    }

    window.setFramerateLimit(60);

    view.setSize(view.getSize() * 0.7f);
    window.setView(view);

    addObject<RockManager>(window, 20); // 0
    auto bm = std::make_unique<BulletManager>(window); // 2
    addObject<Player>(*this, window, *bm.get()); // 1
    objects.push_back(std::move(bm));
}

const sf::Font& GameManager::getFont() const {
    return font;
}

void GameManager::run() {
    // Create delta time
    sf::Clock clock;
    duration time{};


    RockManager& rm = *static_cast<RockManager*>(objects[0].get());
    //Player& player = *static_cast<Player*>(objects[1].get());
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
                    bullet.owner->AddScore(rock.shape+1);
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
