#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <memory>
#include <utility>
#include <vector>

#include <iostream>

class ShapeContainer;

class ShapeWrapperBase {
public:
    ~ShapeWrapperBase() = default;
    // virtual operator sf::Drawable& () = 0;
    // virtual operator sf::Transformable& () = 0;
    virtual operator sf::Shape&() = 0;
};

template<class S>
class ShapeWrapper : public ShapeWrapperBase {
public:
    template<class... Args>
    ShapeWrapper(ShapeContainer& parent, Args&&... args) : m_parent(&parent), m_shape(std::forward<Args>(args)...) {}

    // inline operator sf::Drawable& () override { return m_shape; }
    // inline operator sf::Transformable& () override { return m_shape; }
    inline operator sf::Shape&() override { return m_shape; }
    inline operator S&() { return m_shape; }

    void setPosition(const sf::Vector2f& position);
    sf::Vector2f getPosition() const;

    S& operator*() { return m_shape; }

private:
    ShapeContainer* m_parent;
    S m_shape;
};

class ShapeContainer : public sf::Drawable, public sf::Transformable {
public:
    ShapeContainer() = default;
    inline ShapeContainer(const sf::FloatRect& size) : m_size(size) {}

    template<class S, class... Args>
    decltype(auto) add(Args&&... args) {
        return static_cast<ShapeWrapper<S>&>(*m_objects.emplace_back(std::make_unique<ShapeWrapper<S>>(*this, std::forward<Args>(args)...)));
    }

    inline std::size_t size() const { return m_objects.size(); }
    sf::Shape& operator[](std::size_t index) { return *m_objects[index]; }

    /*
    inline sf::FloatRect getGlobalBounds() const {
        return getTransform().transformRect(getLocalBounds());
    }
    */

private:
    inline void draw(sf::RenderTarget& target, const sf::RenderStates& states) const override {
        auto statesCopy = states;
        // statesCopy.transform *= getTransform();
        // statesCopy.transform *= getTransform();
        // auto trans = getTransform();
        /*
        sf::RectangleShape r(sf::Vector2f(500,500)); // = static_cast<ShapeWrapper<sf::RectangleShape>&>(*m_objects[0]);
        r.setPosition(getPosition());
        static bool x = false;
        if(!x) {
            x = true;
            std::cout << "forced " << r.getPosition() << '\n';
        }
        target.draw(r);
        */

        for(auto& sp : m_objects) {
            target.draw(*sp);
        }
    }

    sf::FloatRect m_size;
    std::vector<std::unique_ptr<ShapeWrapperBase>> m_objects;
};

/*
template<class S>
const sf::Transform& ShapeWrapper<S>::getTransform() const {
    return m_parent->getTransform();
}

template<class S>
const sf::Transform& ShapeWrapper<S>::getInverseTransform() const {
    return m_parent->getInverseTransform();
}
*/

template<class S>
sf::Vector2f ShapeWrapper<S>::getPosition() const {
    return m_shape.getPosition();
}

template<class S>
void ShapeWrapper<S>::setPosition(const sf::Vector2f& position) {
    std::cout << "parent: " << m_parent->getPosition() << '\n';
    std::cout << "rel   : " << position << '\n';
    m_shape.setPosition(m_parent->getPosition() + position);
    std::cout << "me    : " << getPosition() << '\n';
}
