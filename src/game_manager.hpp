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

    const sf::Font& getFont() const;

private:
    sf::RenderWindow window;
    sf::View view;
    sf::Font font;
    std::vector<std::unique_ptr<GameObject>> objects;
    sf::Clock clock;
};
