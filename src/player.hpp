#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "bullet.hpp"
#include "game_object.hpp"
#include "helpers.hpp"
#include "shapecontainer.hpp"
#include "times.hpp"

#include <SFML/Graphics.hpp>

#include <array>
#include <iostream>

class HealthBar : public ShapeContainer {
public:
    inline HealthBar(const sf::FloatRect& br) : ShapeContainer(br) {
        setPosition({br.left + br.width * 2.f / 3.f, br.top});

        // auto& r = add<sf::RectangleShape>(sf::Vector2f{br.width() * 2 / 5.f, br.height() / 20.f});
        // auto& r = add<sf::RectangleShape>(sf::Vector2f{br.width() / 3.f, br.height() / 20});
        // auto& r = add<sf::RectangleShape>(sf::Vector2f{br.width(), br.height()});
        std::cout << "bounds: " << br << '\n';
        auto& r = add<sf::RectangleShape>(sf::Vector2f{500, 500});
        // r.setPosition({br.left/3, br.top / 2});
        // r.setPosition({br.left/3, br.top / 1.f  });
        r.setPosition({0.f, 0.f});
    }
};

class GhostSprite : public sf::Sprite {
public:
    using sf::Sprite::Sprite;

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

    sf::Angle getAngle() const;
    const sf::Vector2f& getPosition() const;
    const sf::Vector2f& getVelocity() const;
    inline float getMass() const { return 10000.f; }
    inline void applyForce(sf::Vector2f impulse) {
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
    sf::Texture normalTexture;
    std::array<sf::Texture, 2> firedTexture;
    GhostSprite player;

    BoundingRect<float> view_bounds;

    sf::Vector2f velocity;

    duration fire_cooldown = 0;
    unsigned score = 0;

    // HealthBar healthbar;
    float health = 10000.f;
};

#endif
