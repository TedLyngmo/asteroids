#include "bullet.hpp"

#include "helpers.hpp"
#include "player.hpp"
#include "pi.hpp"

#include <algorithm>
#include <execution>
#include <cmath>

BulletManager::Bullet::Bullet(sf::Vector2f position, sf::Vector2f speed, float angle) :
    //bullet(sf::Vector2f(8, 8)),
    bullet(4, 6),
    velocity(speed)
{
    bullet.setOrigin(8 / 2, 8 / 2);
    bullet.setFillColor(sf::Color(200, 200, 200));

    float radians = angle * M_PI / 180.f;

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

BulletManager::BulletManager(sf::RenderWindow& window, unsigned MaxBullets) :
    windowptr(&window),
    window_width(window.getSize().x),
    window_height(window.getSize().y),
    max_bullets(MaxBullets)
{
}

void BulletManager::fireBullet(sf::Vector2f position, sf::Vector2f speed, float angle)
{
    if(bullets.size() < max_bullets)
    {
        bullets.emplace_back(position, speed, angle);
    }
}

void BulletManager::Move(duration time)
{
    std::for_each(std::execution::par, bullets.begin(), bullets.end(), [time](Bullet& b) {
        b.Move(time);
    });

    // remove bullets that has moved out of bounds
    auto ne = std::remove_if(std::execution::par, bullets.begin(), bullets.end(), [this](const Bullet& b) {
        auto [x, y] = b.bullet.getPosition();
        return x < 0 || x >= window_width || y < 0 || y >= window_height;
    });
    bullets.erase(ne, bullets.end());
}

void BulletManager::Draw()
{
    for(auto& bullet : bullets) bullet.drawTo(*windowptr);
}
