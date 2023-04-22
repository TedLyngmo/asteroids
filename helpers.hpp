#pragma once

#include <SFML/System/Vector2.hpp>

#include <cmath>
#include <type_traits>

template<class T>
T lengthSquared(const sf::Vector2<T>& v) {
    return v.x * v.x + v.y * v.y;
}

template<class T>
T length(const sf::Vector2<T>& v) {
    return std::sqrt(lengthSquared(v));
}
template<class T>
std::ostream& operator<<(std::ostream& os, const sf::Vector2<T>& v) {
    return os << "sf::Vector2<T>{" << v.x << ',' << v.y << "} l:" << length(v) << " l^2:" << lengthSquared(v);
}

/*
template<class T>
sf::Vector2<T> operator*(const sf::Vector2<T>& v, std::type_identity_t<T> val) {
    return {v.x * val, v.y * val};
}

template<class T>
sf::Vector2<T> operator*(std::type_identity_t<T> val, const sf::Vector2<T>& v) {
    return v * val;
}

template<class T>
sf::Vector2<T> operator/(const sf::Vector2<T>& v, std::type_identity_t<T> val) {
    return {v.x / val, v.y / val};
}

template<class T>
sf::Vector2<T> operator/(std::type_identity_t<T> val, const sf::Vector2<T>& v) {
    return {val / v.x, val / v.y};
}
*/
