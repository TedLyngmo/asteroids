#ifndef BULLET_HPP
#define BULLET_HPP

#include "game_object.hpp"

#include <SFML/Graphics.hpp>

#include <utility>
#include <vector>

class Fx : public GameObject, public sf::Shape {
public:
    Fx(sf::Vector2f position, sf::Vector2f speed, float angle);
};

class FxManager : public GameObject {
public:
    FxManager(sf::RenderWindow& window);

    void AddFx(BulletOwner& owner, sf::Vector2f position, sf::Vector2f speed, float angle);

    void Move(duration time) override;
    void Draw() override;

    inline auto begin() { return bullets.begin(); }
    inline auto end() { return bullets.end(); }

private:
    sf::RenderWindow* windowptr;
    float window_width;
    float window_height;

    std::vector<Bullet> bullets;
};

#endif
