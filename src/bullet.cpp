#include "bullet.hpp"

#include "helpers.hpp"
#include "player.hpp"
#include "pi.hpp"

#include <algorithm>
#include <execution>
#include <cmath>

BulletManager::Bullet::Bullet(BulletOwner& owner, sf::Vector2f position, sf::Vector2f speed, float angle) :
    //bullet(sf::Vector2f(8, 8)),
    bullet(3, 6),
    velocity(speed),
    owner(&owner)
{
    bullet.setOrigin({3.f, 3.f});
    bullet.setFillColor(sf::Color(255, 100, 100));

    float radians = angle * pi / 180.f;

    sf::Vector2f dir{std::cos(radians), std::sin(radians)};

    velocity += 150.f * dir;

    // give it an extra nudge towards the end of the player
    bullet.setPosition(position + 12.5f * dir);
}

void BulletManager::Bullet::Move(duration time)
{
    bullet.move(velocity * time);
}

void BulletManager::Bullet::drawTo(sf::RenderWindow& window)
{
    window.draw(bullet);
}

// -----------------------------------------------------------------------------

BulletManager::BulletManager(sf::RenderWindow& window) :
    windowptr(&window),
    window_width(window.getSize().x),
    window_height(window.getSize().y)
{}

void BulletManager::AddBullet(BulletOwner& owner, sf::Vector2f position, sf::Vector2f speed, float angle)
{
    bullets.emplace_back(owner, position, speed, angle);
}

void BulletManager::Move(duration time)
{
    // remove bullets that has moved out of bounds
    auto newend = std::remove_if(std::execution::par, bullets.begin(), bullets.end(), [this,time](Bullet& b) {
        b.Move(time);
        auto [x, y] = b.bullet.getPosition();
        return x < 0 || x >= window_width || y < 0 || y >= window_height;
    });
    bullets.erase(newend, bullets.end());
}

void BulletManager::Draw()
{
    for(auto& bullet : bullets) bullet.drawTo(*windowptr);
}
