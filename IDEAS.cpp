#include <cmath>
#include <iostream>

namespace sf {

template<class T>
struct Vector2 {
    T x{};
    T y{};
    Vector2& operator+=(const Vector2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    Vector2& operator-=(const Vector2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    Vector2& operator*=(const T rhs) {
        x *= rhs;
        y *= rhs;
        return *this;
    }    
    Vector2& operator/=(const T rhs) {
        x /= rhs;
        y /= rhs;
        return *this;
    }
};
template<class T>
Vector2<T> operator-(const Vector2<T>& inst) {
    return {-inst.x, -inst.y};
}
template<class T>
Vector2<T> operator+(const Vector2<T>& lhs, const Vector2<T>& rhs) {
    return Vector2<T>(lhs) += rhs;
}
template<class T>
Vector2<T> operator-(const Vector2<T>& lhs, const Vector2<T>& rhs) {
    return Vector2<T>(lhs) -= rhs;
}
template<class T>
Vector2<T> operator*(const Vector2<T>& lhs, const T rhs) {
    return Vector2<T>(lhs) *= rhs;
}
template<class T>
Vector2<T> operator*(const T lhs, const Vector2<T>& rhs) {
    return rhs * lhs;
}
template<class T>
Vector2<T> operator/(const Vector2<T>& lhs, const T rhs) {
    return Vector2<T>(lhs) /= rhs;
}

using Vector2f = Vector2<float>;

} // namespace sf

template <typename T>
constexpr T cross(const sf::Vector2<T>& lhs, const sf::Vector2<T>& rhs) {
    return lhs.x * rhs.y - lhs.y * rhs.x;
}

template <typename T>
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

/*
template<class T>
T toRadians(const sf::Vector2<T>& v) {
    return std::atan2(v.y, v.x);
}
*/

class Rock {
public:
    Rock(float radius, float mass) : m_radius(radius), m_mass(mass) {}

    sf::Vector2f getPosition() const { return m_position; }
    sf::Vector2f getVelocity() const { return m_velocity; }
    float getRadius() const { return m_radius; }
    float getMass() const { return m_mass; }
    
    inline void applyForce(const sf::Vector2f impulse) {
        m_velocity += impulse / m_mass;
    }

private:
    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    sf::Vector2f m_force;

    float m_radius;
    float m_mass;
};

bool has_collision(const Rock& lhs, const Rock& rhs) {
    auto r2 = lhs.getRadius() + rhs.getRadius(); // (lhs.getRadius() + rhs.getRadius()) * (lhs.getRadius() + rhs.getRadius())
    r2 *= r2;
    return lengthSquared(lhs.getPosition() - rhs.getPosition()) <= r2;
}

void apply_force_if_collision(Rock& lhs, Rock& rhs) {
    if(has_collision(lhs, rhs)) {
        auto delta = lhs.getPosition() - rhs.getPosition();
        auto distance_squared = lengthSquared(delta);
        auto radius_sum_squared = (lhs.getRadius() + rhs.getRadius()) * (lhs.getRadius() + rhs.getRadius());

        if(distance_squared <= radius_sum_squared) { // objects have collided
            auto collision_normal = normalize(delta);
            auto relative_velocity = lhs.getVelocity() - rhs.getVelocity();
            auto closing_velocity = dot(relative_velocity, collision_normal);

            if(closing_velocity < 0) { // objects are moving towards each other
                auto e = 1.0f; // coefficient of restitution
                auto impulse_magnitude = -(1 + e) * closing_velocity * lhs.getmass() * rhs.getMass() / (lhs.getMass() + rhs.getMass());
                auto impulse = impulse_magnitude * collision_normal;
                lhs.applyForce(impulse);
                rhs.applyForce(-impulse);
            }
        }
    }
}

int main() {

}
