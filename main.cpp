#include "bullet.hpp"
#include "player.hpp"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>

int main()
{
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    auto screenWidth = desktop.width * 3 / 4;
    auto screenHeight = desktop.height * 3 / 4;
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight, desktop.bitsPerPixel), "Asteroids");
    window.setFramerateLimit(60);

    // Create delta time
    sf::Clock clock;
    float dt;

    unsigned max_bullets = 20;
    BulletManager bulletMgr(window, max_bullets);
    Player player(window);

    sf::Event event;
    while (window.isOpen())
    {
        if (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                bulletMgr.fireBullet(player.getPosition(), 10, player.returnAngle());
            }
        }

        //---- moving

        bulletMgr.Move();

        player.Move(dt);

        //---- drawing
        window.clear();

        bulletMgr.Draw();

        dt = clock.restart().asSeconds();

        player.Draw();
        //----

        window.display();
    }
}
