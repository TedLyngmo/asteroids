#include "player.hpp"

#include "game_manager.hpp"
#include "helpers.hpp"
#include "pi.hpp"

#include <SFML/Graphics.hpp>

#include <cmath>
#include <iostream>
#include <random>
#include <stdexcept>

std::size_t GhostSprite::getPointCount() const {
    return 3;
}

sf::Vector2f GhostSprite::getPoint(std::size_t index) const {
    const sf::FloatRect& gb = getLocalBounds();
    const sf::Transform& trans = getTransform();

    switch(index) {
    case 0:
        return trans.transformPoint(sf::Vector2f(gb.left, gb.top));
    case 1:
        return trans.transformPoint(sf::Vector2f(gb.left + gb.width, gb.top + gb.height / 2.f));
    case 2:
        return trans.transformPoint(sf::Vector2f(gb.left, gb.top + gb.height));
    default:
        return {};
    }
}

sf::Texture loadTextureFromFile(const std::string& filename) {
    if(sf::Texture rv; rv.loadFromFile(filename)) return rv;
    throw std::runtime_error("ERROR: Could not loadTextureFromFile \"" + filename + '"');
}

Player::Player(GameManager& gm, sf::RenderWindow& window, BulletManager& bm) :
    gameManagerPtr(&gm), bulletMgr(&bm), windowptr(&window), view(window.getView()), normalTexture(loadTextureFromFile("content/normal.png")),
    firedTexture{loadTextureFromFile("content/fired1.png"), loadTextureFromFile("content/fired2.png")}, player(normalTexture), view_bounds(view) /*, healthbar(view_bounds)*/ {
    player.setScale({.2f, .2f});

    // auto [left, top, pwidth, pheight] = player.getGlobalBounds();
    auto [left, top, pwidth, pheight] = player.getLocalBounds();
    player.setOrigin({pwidth / 2.f, pheight / 2.f});

    auto [wwidth, wheight] = window.getSize();
    player.setPosition({wwidth / 2.f, wheight / 2.f});

    // auto& hb = healthbar.add<sf::RectangleShape>(sf::Vector2f{400,400});
    // hb.setPosition(sf::Vector2f{view_bounds.width() / 2.f, view_bounds.top});
}

sf::Angle Player::getAngle() const {
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

static std::minstd_rand prng(std::random_device{}());
static std::bernoulli_distribution fire_dist{};

void Player::Move(duration time) {
    fire_cooldown -= time;

    if(fire_cooldown < 0.f && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        bulletMgr->AddBullet(*this, getPosition(), getVelocity(), getAngle());
        fire_cooldown += 0.30f;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        player.rotate(sf::radians(-1 * time));
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        player.rotate(sf::radians(1 * time));
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
        player.setTexture(firedTexture[fire_dist(prng)]);
        auto angle = player.getRotation().asRadians();
        float tfact = time * 25;

        auto force = sf::Vector2f(std::cos(angle), std::sin(angle)) * tfact;
        auto newvel = velocity + force;

        // accept new velocity if the speed is less than the threshold or if it at least
        // is less or equal to the old speed:
        if(auto newspeed = lengthSquared(newvel); newspeed < 20000.f) {
            velocity = newvel;
        } else if(auto oldspeed = lengthSquared(velocity); newspeed <= oldspeed) {
            velocity = newvel;
        } else { // no speed increase, but let it change direction towards where the player
                 // is aiming the ship:
            velocity = newvel * 20000.f / oldspeed;
            std::cout << "max speed reached" << std::endl;
        }
    } else {
        player.setTexture(normalTexture);
    }

    // Move in direction player is pointed
    player.move(velocity * time);

    screenWrapping();
}

void Player::screenWrapping() {
    // If player goes beyond x bounds set position to opposite site of screen
    if(player.getPosition().x >= view_bounds.right()) {
        player.setPosition({view_bounds.left, player.getPosition().y});
    } else if(player.getPosition().x < view_bounds.left) {
        player.setPosition({view_bounds.right() - 1, player.getPosition().y});
    }

    // If player goes beyond y bounds set position to opposite site of screen
    if(player.getPosition().y >= view_bounds.bottom()) {
        player.setPosition({player.getPosition().x, view_bounds.top});
    } else if(player.getPosition().y < view_bounds.top) {
        player.setPosition({player.getPosition().x, view_bounds.bottom() - 1});
    }
}

void Player::AddScore(unsigned s) {
    score += s;
}

void Player::Draw() {
    sf::Vector2f wc = view.getCenter();
    sf::Vector2f ws = view.getSize();

    /*
    sf::Text scoret("Score", gameManagerPtr->getFont(), 20);
    scoret.setColor({250u,250u,250u,127u});
    auto scob = scoret.getGlobalBounds();
    scoret.setPosition({wc.x - scob.width/2.f, wc.y - ws.y/2});
    */

    sf::Text num(gameManagerPtr->getFont(), std::to_string(score), 30);
    auto numb = num.getGlobalBounds();
    num.setPosition({wc.x - numb.width / 2.f, wc.y - ws.y / 2});

    // windowptr->draw(scoret);
    windowptr->draw(num);
    // windowptr->draw(healthbar);
    windowptr->draw(player);
}
