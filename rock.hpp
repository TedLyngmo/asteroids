#pragma once

#include "game_object.hpp"

#include <SFML/Graphics.hpp>

#include <array>
#include <utility>
#include <vector>

class RockManager : public GameObject {
public:
    RockManager(sf::RenderWindow& window, unsigned max_rocks);

    void Move(duration time) override;
    void Draw() override;

private:
    void Tick(duration time);

    struct Rock {
        Rock(sf::Vector2f position, float speed, float radians);

        void Move(duration time);
        void drawTo(sf::RenderWindow& window);

        sf::ConvexShape rock;
        sf::Vector2f velocity;
        float angular_velocity;
    };

    sf::RenderWindow* windowptr;
    float window_width;
    float window_height;

    unsigned max_rocks;
    std::vector<Rock> rocks;

    duration time_since_last_spawn{};

    static const std::array<std::pair<sf::ConvexShape, sf::Vector2f>, 3> shapes;
};
