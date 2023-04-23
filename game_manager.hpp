#pragma once

#include "game_object.hpp"

#include <SFML/Graphics.hpp>

#include <memory>
#include <utility>
#include <vector>

class GameManager {
public:
    GameManager();
    void run();

    template<class T, class... Args>
    void addObject(Args&&... args) {
        objects.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
    }

private:
    sf::RenderWindow window;
    unsigned max_bullets = 40;
    unsigned max_rocks = 20;
    std::vector<std::unique_ptr<GameObject>> objects;
    sf::Clock clock;
};
