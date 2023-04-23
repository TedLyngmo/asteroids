#pragma once

#include "game_object.hpp"
#include "shapes.hpp"

#include <SFML/Graphics.hpp>

#include <array>
#include <utility>
#include <vector>

class RockManager : public GameObject {
public:
    RockManager(sf::RenderWindow& window, unsigned max_rocks);

    void Move(duration time) override;
    void Draw() override;

    inline auto begin() {
        return rocks.begin();
    }
    inline auto end() {
        return rocks.end();
    }

private:
    void Tick(duration time);

    struct Rock {
        Rock(sf::Vector2f position, float speed, float radians);

        void Move(duration time);
        void drawTo(sf::RenderWindow& window);
        inline bool isInside(sf::Vector2f pnt) const {
            return rock.isInside(pnt);
        }

        Polygon rock;
        sf::Vector2f velocity;
        float angular_velocity;
    };

    sf::RenderWindow* windowptr;
    float window_width;
    float window_height;

    unsigned max_rocks;
    std::vector<Rock> rocks;

    duration time_since_last_spawn{};

    static const std::array<Polygon, 3> shapes;
};
