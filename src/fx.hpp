#ifndef BULLET_HPP
#define BULLET_HPP

#include "game_object.hpp"

#include <SFML/Graphics.hpp>

#include <utility>
#include <vector>

class FxManager : public GameObject {
public:
    FxManager(sf::RenderWindow& window);

    void AddFx(BulletOwner& owner, sf::Vector2f position, sf::Vector2f speed, float angle);

    void Move(duration time) override;
    void Draw() override;

    inline auto begin() { return bullets.begin(); }
    inline auto end() { return bullets.end(); }

private:
    struct Bullet {
        Bullet(BulletOwner& owner, sf::Vector2f position, sf::Vector2f speed, float angle);

        void Move(duration time);
        void drawTo(sf::RenderWindow& window);
        inline sf::Vector2f getPosition() const { return bullet.getPosition(); }

        //sf::RectangleShape bullet;
        sf::CircleShape bullet;
        sf::Vector2f velocity;
        BulletOwner* owner;
    };

    sf::RenderWindow* windowptr;
    float window_width;
    float window_height;

    std::vector<Bullet> bullets;
};

#endif
