#ifndef BULLET_HPP
#define BULLET_HPP

#include "player.hpp"

#include <SFML/Graphics.hpp>

#include <vector>

class BulletManager {
public:
    BulletManager(sf::RenderWindow& window, unsigned max_bullets);

    void fireBullet(sf::Vector2f position, float speed, float angle);

    void Move();
    void Draw();

private:
    struct Bullet
    {
        Bullet(sf::Vector2f position, float speed, float angle);

        void Move();
        void drawTo(sf::RenderWindow& window);

        sf::RectangleShape bullet;
        sf::Vector2f velocity;
    };

    sf::RenderWindow* windowptr;
    float window_width;
    float window_height;

    unsigned max_bullets;
    std::vector<Bullet> bullets;
};

#endif
