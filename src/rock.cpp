#include "rock.hpp"

#include "helpers.hpp"
#include "pi.hpp"
#include "player.hpp"
#include "shapes.hpp"

#include <algorithm>
#include <cmath>
#include <execution>
#include <iostream>
#include <random>

static std::mt19937_64 no_random(3);                             // make sure the rocks always looks the same
static std::uniform_real_distribution<float> rfactor(.90, 1.10); // to not just have boring circles

sf::Vector2f getVelocity(float speed, float direction_radians) {
    return {speed * std::cos(direction_radians), speed * std::sin(direction_radians)};
}

Polygon make_Polygon(float radius, std::size_t pnts) {
    std::vector<sf::Vector2f> res;
    res.reserve(pnts);
    for(std::size_t i = 0; i < pnts; ++i) {
        float x = radius * std::cos(i * twopi / pnts);
        float y = radius * std::sin(i * twopi / pnts);
        sf::Vector2f pnt(x, y);
        res.push_back(sf::Vector2f(radius, radius) + pnt * rfactor(no_random));
    }
    return {res.begin(), res.end()};
}

const std::array<Polygon, 8> RockManager::shapes{{
    make_Polygon(191, 31),
    make_Polygon(179, 19),
    make_Polygon(159, 17),
    make_Polygon(126, 15),
    make_Polygon(88, 13),
    make_Polygon(50, 11),
    make_Polygon(32, 9),
    make_Polygon(15, 7),
}};

static std::mt19937_64 prng(std::random_device{}());
static std::uniform_real_distribution<float> rnd_angle_dist(0, 360);
static std::uniform_real_distribution<float> rnd_radians_dist(-pi, pi);
static std::uniform_real_distribution<float> rnd_radians45_dist(-pi/4.f, pi/4.f);
static std::uniform_real_distribution<float> rnd_angvel_dist(-27.5f, 27.5f);
static std::uniform_real_distribution<float> rnd_color_dist(-5, 5);

RockManager::Rock::Rock(sf::Vector2f position, sf::Vector2f velocity, float angular_velocity, unsigned shape_no) :
    rock(RockManager::shapes[shape_no]), velocity(velocity), angular_velocity(angular_velocity), shape{shape_no} {
    rock.setRotation(rnd_angle_dist(prng));

    rock.setFillColor(sf::Color(120 + shape_no * 10 + rnd_color_dist(prng), 120 + shape_no * 10 + rnd_color_dist(prng), 120 + shape_no * 10 + rnd_color_dist(prng)));

    rock.setPosition(position);
}

void RockManager::Rock::Move(duration time) {
    rock.rotate(angular_velocity * time);
    rock.move(velocity * time);
}

void RockManager::Rock::drawTo(sf::RenderWindow& window) {
    window.draw(rock);
}

void RockManager::Rock::aimTowards(sf::Vector2f pos) {
    auto speed = length(velocity);
    if(speed < 5.f) speed = 5.f;

    auto aim = toRadians(pos - rock.getPosition());
    velocity = getVelocity(speed, aim + rnd_radians45_dist(prng));
}

// -----------------------------------------------------------------------------

void RockManager::hit(Rock& rock) {
    rock.is_hit = true;
}

static inline float length(const sf::Vector2f& v) {
    return std::sqrt(v.x * v.x + v.y + v.y);
}

void RockManager::update() {
    std::vector<Rock> new_rocks;

    std::erase_if(rocks, [&](Rock& rock) {
        if(not rock.is_hit) return false;

        if(rock.shape + 1 < RockManager::shapes.size()) {
            auto velocity = rock.velocity * 1.1f + getVelocity(3, rnd_radians_dist(prng));
            auto ang_speed = rnd_angvel_dist(prng);
            AddRockTo(new_rocks, rock.rock.getPosition(), velocity, ang_speed, rock.shape + 1);
            // if(rock.shape + 2 < RockManager::shapes.size())
            AddRockTo(new_rocks, rock.rock.getPosition(), velocity, ang_speed, rock.shape + 1);
        }
        return true;
    });
    for(auto& r : new_rocks) r.Move(duration{0.3});
    rocks.insert(rocks.end(), new_rocks.begin(), new_rocks.end());
}

RockManager::RockManager(sf::RenderWindow& window, unsigned MaxRocks) : windowptr(&window), window_width(window.getSize().x), window_height(window.getSize().y), max_rocks(MaxRocks) {}

void RockManager::Tick(duration time) {
    if(std::chrono::steady_clock::now() > stop_spawning_at) return;

    constexpr float time_between_rocks = 30.f; // seconds
    if((time_since_last_spawn += time) > time_between_rocks) {
        time_since_last_spawn -= time_between_rocks;
        AddRock();
    }
}

void RockManager::Move(duration time) {
    Tick(time);

    std::for_each(std::execution::par, rocks.begin(), rocks.end(), [this, time](Rock& r) {
        r.Move(time);

        auto [x, y] = r.rock.getPosition();

        if(x > window_width) {
            r.rock.setPosition(0, y);
            r.aimTowards({window_width / 2, window_height / 2});
        } else if(x < 0) {
            r.rock.setPosition(window_width - 1, y);
            r.aimTowards({window_width / 2, window_height / 2});
        }

        if(y >= window_height) {
            r.rock.setPosition(x, 0);
            r.aimTowards({window_width / 2, window_height / 2});
        } else if(y < 0) {
            r.rock.setPosition(x, window_height - 1);
            r.aimTowards({window_width / 2, window_height / 2});
        }
    });
}

void RockManager::AddRockTo(std::vector<Rock>& vec, sf::Vector2f position, sf::Vector2f velocity, float angular_velocity, unsigned shape_no) {
    vec.emplace_back(position, velocity, angular_velocity, shape_no);
}

void RockManager::AddRock() {
    static std::uniform_real_distribution<float> xd(0, window_width);
    static std::uniform_real_distribution<float> yd(0, window_height);
    static std::uniform_int_distribution<int> side(0, 3);

    sf::Vector2f position;

    switch(side(prng)) {
    case 0:
        position = {-10, yd(prng)};
        break;
    case 1:
        position = {window_width - 1 + 10, yd(prng)};
        break;
    case 2:
        position = {xd(prng), -10};
        break;
    case 3:
        position = {xd(prng), window_height - 1 + 10};
        break;
    }

    rocks.emplace(rocks.begin(), position, getVelocity(10, rnd_radians_dist(prng)), rnd_angvel_dist(prng) / 8, 0);
}

void RockManager::Draw() {
    for(auto& rock : rocks) rock.drawTo(*windowptr);
}
