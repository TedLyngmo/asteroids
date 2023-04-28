#include "player.hpp"

#include "helpers.hpp"
#include "game_manager.hpp"
#include "pi.hpp"

#include <SFML/Graphics.hpp>

#include <cmath>
#include <iostream>
#include <stdexcept>

sf::Texture loadTextureFromFile(const std::string& filename) {
    sf::Texture rv;
    if(!rv.loadFromFile(filename)) {
        throw std::runtime_error("ERROR: Could not loadTextureFromFile \"" + filename + '"');
    }
    return rv;
}

Player::Player(GameManager& gm, sf::RenderWindow& window, BulletManager& bm) :
    gameManagerPtr(&gm),
    bulletMgr(&bm),
    windowptr(&window),
    view(window.getView()),
    normalTexture(loadTextureFromFile("content/normal.png")),
    firedTexture(loadTextureFromFile("content/fired.png")),
    view_bounds(view)
{
    player.setTexture(normalTexture);
    player.setScale({.2f, .2f});

    //auto [left, top, pwidth, pheight] = player.getGlobalBounds();
    auto [left, top, pwidth, pheight] = player.getLocalBounds();
    player.setOrigin({pwidth / 2.f, pheight / 2.f});

    auto[wwidth, wheight] = window.getSize();
    player.setPosition({wwidth / 2.f, wheight / 2.f});
}

float Player::getAngle() const
{
    return player.getRotation();
}

const sf::Vector2f& Player::getPosition() const {
    return player.getPosition();
}

const sf::Vector2f& Player::getVelocity() const {
    return velocity;
}

bool Player::handleEvent([[maybe_unused]] const sf::Event& ev) {
    return false;
}


void Player::Move(duration time)
{
    fire_cooldown -= time;

    if (fire_cooldown < 0.f && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        bulletMgr->AddBullet(*this, getPosition(), getVelocity(), getAngle());
        fire_cooldown += 0.30f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        player.rotate(-70 * time);
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        player.rotate(70 * time);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        player.setTexture(firedTexture);
        auto angle = player.getRotation() * pi / 180;
        float tfact = time * 25;
        velocity += sf::Vector2f(std::cos(angle), std::sin(angle)) * tfact;
        auto speed2 = lengthSquared(velocity); // velocity.lengthSq() in SFML3
        if(speed2 > 20000.f) {
            std::cout << "max speed reached" << std::endl;
            velocity *= 20000.f / speed2;
        }
    }
    else
    {
        player.setTexture(normalTexture);
    }
    // Move in direction player is pointed
    player.move(velocity * time);

    screenWrapping();
}

void Player::screenWrapping()
{
    // If player goes beyond x bounds set position to opposite site of screen
    if (player.getPosition().x >= view_bounds.right)
    {
        player.setPosition({view_bounds.left, player.getPosition().y});
    }
    else if (player.getPosition().x < view_bounds.left)
    {
        player.setPosition({view_bounds.right - 1, player.getPosition().y});
    }

    // If player goes beyond y bounds set position to opposite site of screen
    if (player.getPosition().y >= view_bounds.bottom)
    {
        player.setPosition({player.getPosition().x, view_bounds.top});
    }
    else if (player.getPosition().y < view_bounds.top)
    {
        player.setPosition({player.getPosition().x, view_bounds.bottom - 1});
    }
}

void Player::AddScore(unsigned s) {
    score += s;
}

void Player::Draw()
{
    sf::Text txt(std::to_string(score), gameManagerPtr->getFont(), 30);
    auto tb = txt.getGlobalBounds();

    sf::Vector2f wc = view.getCenter();
    sf::Vector2f ws = view.getSize();

    txt.setPosition({wc.x - tb.width/2.f, wc.y - ws.y/2});

    windowptr->draw(txt);
    windowptr->draw(player);
}
