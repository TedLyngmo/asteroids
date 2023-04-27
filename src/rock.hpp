#pragma once

#include "game_object.hpp"

#include "shapes.hpp"
#include "times.hpp"

#include <SFML/Graphics.hpp>

#include <array>
#include <utility>
#include <vector>

class RockManager : public GameObject {
    struct Rock {
        Rock(sf::Vector2f position, sf::Vector2f velocity, float angular_velocity, unsigned shape_no);

        void Move(duration time);
        void drawTo(sf::RenderWindow& window);
        inline bool isInside(sf::Vector2f pnt) const {
            return rock.isInside(pnt);
        }

        Polygon rock;
        sf::Vector2f velocity;
        float angular_velocity;
        unsigned shape;
        bool is_hit = false;
    };

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

    void hit(Rock& r);

    void update();

    template<class Func>
    void erase_if(Func&& func) {
        std::erase_if(rocks, std::forward<Func>(func));
    }

private:
    void AddRock();
    void AddRockTo(std::vector<Rock>& vec, sf::Vector2f position, sf::Vector2f velocity, float angular_speed, unsigned shape_no);
    void Tick(duration time);

    sf::RenderWindow* windowptr;
    float window_width;
    float window_height;

    unsigned max_rocks;
    std::vector<Rock> rocks;

    duration time_since_last_spawn = 30 * 3; // spawn-a-lot
    std::chrono::steady_clock::time_point stop_spawning_at = std::chrono::steady_clock::now() + std::chrono::seconds(60);

    static const std::array<Polygon, 8> shapes;
};
