#include "rock.hpp"

#include "helpers.hpp"
#include "player.hpp"
#include "pi.hpp"
#include "shapes.hpp"

#include <algorithm>
#include <execution>
#include <random>
#include <cmath>
#include <iostream>

const std::array<Polygon, 3> RockManager::shapes{{
    {{0,10}, {50,0}, {120,10}, {100,90}, {30,100}, {0,50}},
    {{30,0}, {47, 10}, {44, 20}, {60,10}, {70,20}, {60,30}, {63,42}, {47,40}, {40,50}, {30,40}, {20,40}, {10, 30}, {24, 22}},
    {{0,10}, {50,0}, {120,10}, {100,90}, {30,100}, {0,50}},
}};

static std::mt19937_64 prng(std::random_device{}());
static std::uniform_real_distribution<float> rnd_angle_dist(0, 360);
static std::uniform_real_distribution<float> rnd_radians_dist(0, M_PI * 2);
static std::uniform_real_distribution<float> rnd_angvel_dist(-90, 90);

RockManager::Rock::Rock(sf::Vector2f position, float speed, float radians) :
    rock(RockManager::shapes[0]),
    velocity(speed * std::cos(radians), speed * std::sin(radians))
{
    rock.setRotation(rnd_angle_dist(prng));

    rock.setFillColor(sf::Color(200, 200, 200));
    angular_velocity = rnd_angvel_dist(prng);

    rock.setPosition(position);
}

void RockManager::Rock::Move(duration time)
{
    rock.rotate(angular_velocity * time);
    rock.move(velocity * time);
}

void RockManager::Rock::drawTo(sf::RenderWindow& window)
{
    window.draw(rock);
}

// -----------------------------------------------------------------------------

RockManager::RockManager(sf::RenderWindow& window, unsigned MaxRocks) :
    windowptr(&window),
    window_width(window.getSize().x),
    window_height(window.getSize().y),
    max_rocks(MaxRocks)
{
}

void RockManager::Move(duration time)
{
    Tick(time);

    std::for_each(std::execution::par, rocks.begin(), rocks.end(), [this, time](Rock& r){
        r.Move(time);

        auto [x, y] = r.rock.getPosition();

        if (x > window_width)
            r.rock.setPosition(0, y);
        else if (x < 0)
            r.rock.setPosition(window_width - 1, y);

        if (y >= window_height)
            r.rock.setPosition(x, 0);
        else if (y < -10)
            r.rock.setPosition(x, window_height - 1);
    });
}

void RockManager::Tick(duration time) {
    static std::uniform_real_distribution<float> xd(0, window_width);
    static std::uniform_real_distribution<float> yd(0, window_height);
    static std::uniform_int_distribution<int> side(0, 3);
    if((time_since_last_spawn += time) > 10) {
        time_since_last_spawn -= 10;

        sf::Vector2f position;

        switch(side(prng)) {
        case 0: position = {-10, yd(prng)}; break;
        case 1: position = {window_width - 1 + 10, yd(prng)}; break;
        case 2: position = {xd(prng), -10}; break;
        case 3: position = {xd(prng), window_height - 1 + 10}; break;
        }

        rocks.emplace_back(position, 10, rnd_radians_dist(prng));
    }
}

void RockManager::Draw()
{
    for(auto& rock : rocks) rock.drawTo(*windowptr);
}
