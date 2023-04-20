#include "player.hpp"
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

Player::Player(sf::RenderWindow& window) :
    windowptr(&window),
    window_width(window.getSize().x),
    window_height(window.getSize().y),
    normalTexture(loadTextureFromFile("content/normal.png")),
    firedTexture(loadTextureFromFile("content/fired.png"))

{
    player.setTexture(normalTexture);
    player.setScale(.2, .2);

    //auto [left, top, pwidth, pheight] = player.getGlobalBounds();
    auto [left, top, pwidth, pheight] = player.getLocalBounds();
    player.setOrigin(pwidth / 2, pheight / 2);

    auto[wwidth, wheight] = window.getSize();
    player.setPosition(wwidth / 2, wheight / 2);
}

const sf::Vector2f& Player::getPosition() const {
    return player.getPosition();
}

void Player::Move(float dt)
{
    int angle = player.getRotation() * M_PI / 180;
    // Accelerate on button press
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        player.setTexture(firedTexture);

        while (acceleration.y <= 1.0f)
        {
            acceleration.y += acc;
            acceleration.x += acc;
        }
        velocity += acceleration;
        velocity *= 0.50f;
        direction.x += velocity.x * std::cos(angle);
        direction.y += velocity.y * std::sin(angle);
    }
    // Decelerate on button release
    else
    {
        player.setTexture(normalTexture);

        velocity -= acceleration;
        if (velocity.x <= 0.3f)
        {
            velocity.x = 0;
        }

        if (velocity.y <= 0.3f)
        {
            velocity.y = 0;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        player.rotate(-4);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        player.rotate(4);
    }

    // Move in direction player is pointed
    player.move(direction * dt);

    screenWrapping();
}

void Player::screenWrapping()
{
    // If player goes beyond x bounds set position to opposite site of screen
    if (player.getPosition().x > window_width)
    {
        player.setPosition(0, player.getPosition().y);
    }
    else if (player.getPosition().x <= 0)
    {
        player.setPosition(window_width, player.getPosition().y);
    }

    // If player goes beyond y bounds set position to opposite site of screen
    if (player.getPosition().y >= window_height)
    {
        player.setPosition(player.getPosition().x, 0);
    }
    else if (player.getPosition().y <= 0)
    {
        player.setPosition(player.getPosition().x, window_height);
    }
}

float Player::returnAngle()
{
    return player.getRotation();
}

void Player::Draw()
{
    windowptr->draw(player);
}
