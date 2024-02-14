#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iterator>
#include <optional>
#include <ostream>
#include <type_traits>

template<class T>
struct BoundingRect {
    BoundingRect() = default;
    explicit constexpr BoundingRect(const sf::Rect<T>& r) : left(r.left), top(r.top), right(r.left + r.width), bottom(r.top + r.height) {}
    explicit constexpr BoundingRect(const sf::Vector2<T>& center, const sf::Vector2<T>& size) : BoundingRect(sf::Rect<T>(sf::Vector2<T>(center.x - size.x / 2.f, center.y - size.y / 2.f), sf::Vector2<T>(size.x, size.y))) {}
    explicit constexpr BoundingRect(const sf::View& view) : BoundingRect(view.getCenter(), view.getSize()) {}

    BoundingRect& operator=(const sf::Rect<T>& r) {
        *this = BoundingRect(r);
        return *this;
    }
    BoundingRect& operator=(const sf::View& v) {
        *this = BoundingRect(v);
        return *this;
    }

    T left;
    T top;
    T right;
    T bottom;
};

template<typename T>
constexpr T cross(const sf::Vector2<T>& lhs, const sf::Vector2<T>& rhs) {
    return lhs.x * rhs.y - lhs.y * rhs.x;
}

template<typename T>
constexpr T dot(const sf::Vector2<T>& lhs, const sf::Vector2<T>& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

template<class T>
T lengthSquared(const sf::Vector2<T>& v) {
    return dot(v, v);
}

template<class T>
T length(const sf::Vector2<T>& v) {
    return std::sqrt(lengthSquared(v));
}

template<class T>
T distanceSquared(const sf::Vector2<T>& lhs, const sf::Vector2<T>& rhs) {
    return lengthSquared(lhs - rhs);
}

template<class T>
T distance(const sf::Vector2<T>& lhs, const sf::Vector2<T>& rhs) {
    return std::sqrt(distanceSquared(lhs, rhs));
}

template<class T>
sf::Vector2<T> normalize(const sf::Vector2<T>& v) {
    return v / length(v);
}

template<class T>
T toRadians(const sf::Vector2<T>& v) {
    return std::atan2(v.y, v.x);
}

template<class T>
struct Line {
    sf::Vector2<T> A;
    sf::Vector2<T> B;
};

template<class T>
std::ostream& operator<<(std::ostream& os, const sf::Vector2<T>& v) {
    return os << "sf::Vector2<T>{" << v.x << ',' << v.y << '}'; // "} length:" << length(v) << " length^2:" << lengthSquared(v);
}

template<class T>
std::ostream& operator<<(std::ostream& os, const Line<T>& l) {
    return os << "Line<T>{{" << l.A << "}, {" << l.B << "}}";
}

template<class T>
std::ostream& operator<<(std::ostream& os, const sf::Rect<T>& r) {
    return os << "sf::Rect<T>{" << r.left << ',' << r.top << ',' << r.width << ',' << r.height << '}';
}

inline sf::Vector2f getCentroid(const sf::Shape& shape) {
    const auto count = shape.getPointCount();

    switch(count) {
    case 0:
        assert(false);
        return {};
    case 1:
        return shape.getPoint(0);
    case 2:
        return (shape.getPoint(0) + shape.getPoint(1)) / 2.f;
    default: // more than two points
        sf::Vector2f centroid;
        float twiceArea = 0;

        auto previousPoint = shape.getPoint(count - 1);
        for(std::size_t i = 0; i < count; ++i) {
            const auto currentPoint = shape.getPoint(i);
            const float product = cross(previousPoint, currentPoint);
            twiceArea += product;
            centroid += (currentPoint + previousPoint) * product;

            previousPoint = currentPoint;
        }

        if(twiceArea != 0.f) {
            return centroid / 3.f / twiceArea;
        }

        // Fallback for no area - find the center of the bounding box
        auto minPoint = shape.getPoint(0);
        auto maxPoint = minPoint;
        for(std::size_t i = 1; i < count; ++i) {
            const auto currentPoint = shape.getPoint(i);
            minPoint.x = std::min(minPoint.x, currentPoint.x);
            maxPoint.x = std::max(maxPoint.x, currentPoint.x);
            minPoint.y = std::min(minPoint.y, currentPoint.y);
            maxPoint.y = std::max(maxPoint.y, currentPoint.y);
        }
        return (maxPoint + minPoint) / 2.f;
    }
}

template<class T>
constexpr bool line_intersects(const Line<T>& ln1, const Line<T>& ln2) {
    constexpr T Zero{};
    // Calculate the cross product of the vectors formed by each line.
    const sf::Vector2<T> P2BA = ln2.B - ln2.A;
    const T crossProduct1 = cross(P2BA, ln1.A - ln2.A);
    const T crossProduct2 = cross(P2BA, ln1.B - ln2.A);

    // Check if the signs of the cross products are different and
    // if the intersection point lies within both line segments.
    if((crossProduct1 > Zero && crossProduct2 < Zero) || (crossProduct1 < Zero && crossProduct2 > Zero)) {
        const sf::Vector2<T> P1BA = ln1.B - ln1.A;
        const T crossProduct3 = cross(P1BA, ln2.A - ln1.A);
        const T crossProduct4 = cross(P1BA, ln2.B - ln1.A);

        if((crossProduct3 > Zero && crossProduct4 < Zero) || (crossProduct3 < Zero && crossProduct4 > Zero)) {
            return true;
        }
    }

    return false;
}

template<class MassObject1, class MassObject2>
constexpr sf::Vector2f getCollisionImpulseVector(MassObject1& lhs, MassObject2& rhs, float e = 1.0f) { // e = coefficient of restitution
    const auto delta = lhs.getPosition() - rhs.getPosition();
    // const auto distance_squared = lengthSquared(delta);
    // const auto radius_sum_squared = (lhs.getRadius() + rhs.getRadius()) * (lhs.getRadius() + rhs.getRadius());

    // if(distance_squared <= radius_sum_squared) { // objects have collided
    const auto collision_normal = normalize(delta);
    const auto relative_velocity = lhs.getVelocity() - rhs.getVelocity();
    const auto closing_velocity = dot(relative_velocity, collision_normal);

    if(closing_velocity < 0.f) { // objects are moving towards each other
        const auto impulse_magnitude = -(1.f + e) * closing_velocity * lhs.getMass() * rhs.getMass() / (lhs.getMass() + rhs.getMass());
        const auto impulse = impulse_magnitude * collision_normal;
        return impulse;
    }
    //}
    return {};
}
