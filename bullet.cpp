#include "bullet.hpp"
#include "player.hpp"
#include "pi.hpp"

#include <algorithm>
#include <cmath>

BulletManager::Bullet::Bullet(sf::Vector2f position, float speed, float angle)
{
    bullet.setSize(sf::Vector2f(8, 8));
    bullet.setOrigin(8 / 2, 8 / 2);
    bullet.setFillColor(sf::Color(200, 200, 200));

    auto playerDirX = angle * M_PI / 180.f;
    auto playerDirY = angle * M_PI / 180.f;

    velocity.x = speed * std::cos(playerDirX);
    velocity.y = speed * std::sin(playerDirY);

    // give it an extra nudge towards the end of the player
    bullet.setPosition(position + velocity);
}

void BulletManager::Bullet::Move()
{
    bullet.move(velocity);
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

void BulletManager::fireBullet(sf::Vector2f position, float speed, float angle)
{
    if(bullets.size() < max_bullets)
    {
        bullets.emplace_back(position, speed, angle);
    }
}

void BulletManager::Move()
{
    for(auto& bullet : bullets) bullet.Move();

    // remove bullets that has moved out of bounds
    std::erase_if(bullets, [this](const Bullet& bullet) {
        auto [x, y] = bullet.bullet.getPosition();
        return x < 0 || x >= window_width || y < 0 || y >= window_height;
    });
}

void BulletManager::Draw()
{
    for(auto& bullet : bullets) bullet.drawTo(*windowptr);
}
