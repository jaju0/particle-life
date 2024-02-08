#ifndef PARTICLE_LIFE_MATH_HELPER_HPP_
#define PARTICLE_LIFE_MATH_HELPER_HPP_

#include <cstdint>
#include <cmath>
#include <SFML/Graphics.hpp>

namespace particle_life {

inline float dot(sf::Vector2f p)
{
    return  p.x * p.x + p.y * p.y;
}

inline float magnitude(sf::Vector2f p)
{
    return std::sqrt(dot(p));
}

inline sf::Vector2f normalize(const sf::Vector2f v)
{
    float mag = magnitude(v);
    return mag == 0.0f ? sf::Vector2f() : v / mag;
}

inline bool rectContainsOrTouches(const sf::FloatRect& rect, const sf::Vector2f p)
{
    return (
        p.x >= rect.left &&
        p.x <= rect.left + rect.width &&
        p.y >= rect.top &&
        p.y <= rect.top + rect.height
    );
}

inline bool lineIntersectsCircle(sf::Vector2f center, float radius, sf::Vector2f p1, sf::Vector2f p2)
{
    p1 -= center;
    p2 -= center;

    sf::Vector2f d(p2.x-p1.x, p2.y-p1.y);
    float dr = std::sqrt(d.x * d.x + d.y * d.y);
    float D = p1.x * p2.y - p2.x * p1.y;
    float discriminant = radius * radius * dr * dr - D * D;

    return discriminant >= 0.0f;
}

inline bool rectangleIntersectsCircle(sf::Vector2f center, float radius, sf::FloatRect rect)
{
    sf::Vector2f A(rect.left, rect.top);
    sf::Vector2f B(rect.left + rect.width, rect.top);
    sf::Vector2f C(rect.left + rect.width, rect.top + rect.height);
    sf::Vector2f D(rect.left, rect.top + rect.height);

    return (
        rect.contains(center) ||
        lineIntersectsCircle(center, radius, A, B) ||
        lineIntersectsCircle(center, radius, B, C) ||
        lineIntersectsCircle(center, radius, C, D) ||
        lineIntersectsCircle(center, radius, D, A)
    );
}

inline bool isPointInCircle(sf::Vector2f center, float radius, sf::Vector2f p)
{
    auto diff = center - p;
    return magnitude(diff) <= radius;
}

}

#endif