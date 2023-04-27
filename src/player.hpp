#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "bullet.hpp"
#include "helpers.hpp"
#include "game_object.hpp"
#include "times.hpp"

#include <SFML/Graphics.hpp>

class GameManager;

class Player : public BulletOwner {
public:
    Player(GameManager&, sf::RenderWindow& window, BulletManager&);

    void Move(duration time) override;
    void Draw() override;
    void AddScore(unsigned score) override;

    float getAngle() const;
    const sf::Vector2f& getPosition() const;
    const sf::Vector2f& getVelocity() const;

    bool handleEvent(const sf::Event&);

private:
    void screenWrapping();

    GameManager* gameManagerPtr;
    BulletManager* bulletMgr;
    sf::RenderWindow* windowptr;
    sf::View view;
    sf::Sprite player;
    sf::Texture normalTexture;
    sf::Texture firedTexture;

    BoundingRect<float> view_bounds;

    sf::Vector2f velocity;

    duration fire_cooldown = 0;
    unsigned score = 0;
};

#endif
