#ifndef BULLET_HPP
#define BULLET_HPP

#include "game_object.hpp"

#include <SFML/Graphics.hpp>

#include <utility>
#include <vector>

class BulletManager : public GameObject {
public:
    BulletManager(sf::RenderWindow& window);

    void AddBullet(BulletOwner& owner, sf::Vector2f position, sf::Vector2f speed, sf::Angle angle);

    void Move(duration time) override;
    void Draw() override;

    template<class Func>
    void erase_if(Func&& func) {
        std::erase_if(bullets, std::forward<Func>(func));
    }

    inline auto begin() { return bullets.begin(); }
    inline auto end() { return bullets.end(); }

private:
    struct Bullet {
        Bullet(BulletOwner& owner, sf::Vector2f position, sf::Vector2f speed, sf::Angle angle);

        void Move(duration time);
        void drawTo(sf::RenderWindow& window);
        inline sf::Vector2f getPosition() const { return bullet.getPosition(); }

        // sf::RectangleShape bullet;
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
