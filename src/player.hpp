#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "bullet.hpp"
#include "helpers.hpp"
#include "game_object.hpp"
#include "times.hpp"

#include <SFML/Graphics.hpp>

#include <array>
#include <iostream>

class GhostSprite : public sf::Sprite {
public:
     std::size_t getPointCount() const;
     sf::Vector2f getPoint(std::size_t index) const;
};

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
    inline float getMass() const { return 10000.f; }
    inline void  applyForce(sf::Vector2f impulse) {
        impulse = impulse / getMass();
        auto damage = (length(impulse) + 200.f) / 2.f;
        health -= damage;
        std::cout << "Damage: " << damage << "  Player health: " << static_cast<long long>(health) << std::endl;
        velocity += impulse;
    }

    bool handleEvent(const sf::Event&);

    inline std::size_t getPointCount() const { return player.getPointCount(); }
    inline sf::Vector2f getPoint(std::size_t index) const { return player.getPoint(index); }
    inline sf::FloatRect getGlobalBounds() const { return player.getGlobalBounds(); }

    inline bool isAlive() const { return health > 0.f; }

private:
    void screenWrapping();

    GameManager* gameManagerPtr;
    BulletManager* bulletMgr;
    sf::RenderWindow* windowptr;
    sf::View view;
    GhostSprite player;
    sf::Texture normalTexture;
    std::array<sf::Texture, 2> firedTexture;

    BoundingRect<float> view_bounds;

    sf::Vector2f velocity;

    duration fire_cooldown = 0;
    unsigned score = 0;
    float health = 10000.f;
};

#endif
