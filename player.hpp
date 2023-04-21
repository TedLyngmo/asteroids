#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "bullet.hpp"
#include "game_object.hpp"
#include "times.hpp"

#include <SFML/Graphics.hpp>

class Player : public GameObject {
public:
    Player(sf::RenderWindow& window);

    void Move(duration time) override;
    void Draw() override;

    float getAngle() const;
    const sf::Vector2f& getPosition() const;
    const sf::Vector2f& getVelocity() const;

    bool handleEvent(const sf::Event&);

private:
    void screenWrapping();

    sf::RenderWindow* windowptr;
    float window_width;
    float window_height;
    sf::Sprite player;
    sf::Texture normalTexture;
    sf::Texture firedTexture;

    const float acc = 0.1f;
    const float dAcc = 1.0f;

    sf::Vector2f velocity;

    BulletManager bulletMgr;
};

#endif
