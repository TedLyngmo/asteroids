#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

class Player
{
public:
    Player(sf::RenderWindow& window);
    void Draw();
    void Move(float dt);
    float returnAngle();

    const sf::Vector2f& getPosition() const;

private:
    void screenWrapping();

    sf::RenderWindow* windowptr;
    float window_width;
    float window_height;
    sf::Sprite player;
    sf::Vector2f direction;
    sf::Texture normalTexture;
    sf::Texture firedTexture;

    const float acc = 0.1f;
    const float dAcc = 1.0f;

    sf::Vector2f acceleration;
    sf::Vector2f velocity;
    float x = 512;
    float y = 394;
};

#endif
