#ifndef BULLET_HPP
#define BULLET_HPP

#include "game_object.hpp"

#include <SFML/Graphics.hpp>

#include <vector>

class BulletManager : public GameObject {
public:
    BulletManager(sf::RenderWindow& window, unsigned max_bullets);

    void fireBullet(sf::Vector2f position, sf::Vector2f speed, float angle);

    void Move(duration time) override;
    void Draw() override;

private:
    struct Bullet {
        Bullet(sf::Vector2f position, sf::Vector2f speed, float angle);

        void Move(duration time);
        void drawTo(sf::RenderWindow& window);

        //sf::RectangleShape bullet;
        sf::CircleShape bullet;
        sf::Vector2f velocity;
    };

    sf::RenderWindow* windowptr;
    float window_width;
    float window_height;

    unsigned max_bullets;
    std::vector<Bullet> bullets;
};

#endif
