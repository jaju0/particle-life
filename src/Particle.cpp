#include <iostream>
#include <cassert>
#include <cmath>

#include "mathhelper.hpp"
#include "Particle.hpp"

namespace particle_life {

std::vector<sf::Color> Particle::s_colors = std::vector<sf::Color>();
std::vector<float> Particle::s_shapeRadii = std::vector<float>();
std::vector<float> Particle::s_masses = std::vector<float>();
std::vector<std::vector<float>> Particle::s_radii = std::vector<std::vector<float>>();
std::vector<std::vector<float>> Particle::s_minDistances = std::vector<std::vector<float>>();
std::vector<std::vector<float>> Particle::s_forces = std::vector<std::vector<float>>();
float Particle::s_repulsiveForceMultiplier = 2.0f;


Particle::Particle()
    : Particle({0, 0}, 0)
{}

Particle::Particle(const sf::Vector2f& position, uint32_t typeId)
    : m_typeId(typeId)
    , m_shape(s_shapeRadii.at(typeId))
    , m_position(position)
    , m_velocity(0.0f)
{
    m_shape.setFillColor(s_colors.at(typeId));
    m_shape.setPosition(m_position);
}

void Particle::update(float deltaTime, float speed, float frictionFactor, float dampingFactor, std::vector<Particle*>& neighbours, Particle* pParticleLastFrame, sf::Vector2f planeSize)
{
    float mass = s_masses.at(m_typeId);
    sf::Vector2f newPosition(m_position);
    for(auto neighbour : neighbours)
    {
        if(neighbour == pParticleLastFrame)
            continue;

        uint32_t neighbourTypeId = neighbour->getTypeId();
        float radius = s_radii.at(m_typeId).at(neighbourTypeId);
        float minDistance = s_minDistances.at(m_typeId).at(neighbourTypeId);
        float maxForce = s_forces.at(m_typeId).at(neighbourTypeId);

        sf::Vector2f diff = neighbour->getPosition() - m_position;
        if(diff.x > 0.5f * planeSize.x)
            diff.x -= planeSize.x;
        else if(diff.x < -0.5f * planeSize.x)
            diff.x += planeSize.x;
        if(diff.y > 0.5f * planeSize.y)
            diff.y -= planeSize.y;
        else if(diff.y < -0.5f * planeSize.y)
            diff.y += planeSize.y;
        
        float length = magnitude(diff);

        if(length > radius)
            continue;

        sf::Vector2f dir = normalize(diff);

        float force = dampingFactor * calculateForce(length, minDistance, radius, maxForce);
        float acceleration = force / mass;
        m_velocity += acceleration;
        newPosition += dir * m_velocity * speed * deltaTime;
        m_velocity *= frictionFactor;
    }

    if(newPosition.x < 0.0f)
        newPosition.x += planeSize.x;
    else if(newPosition.x > planeSize.x)
        newPosition.x -= planeSize.x;
    if(newPosition.y < 0.0f)
        newPosition.y += planeSize.y;
    else if(newPosition.y > planeSize.y)
        newPosition.y -= planeSize.y;

    setPosition(newPosition);
}

void Particle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_shape, states);
}

uint32_t Particle::getTypeId()
{
    return m_typeId;
}

const sf::Vector2f& Particle::getPosition() const
{
    return m_position;
}

void Particle::setPosition(const sf::Vector2f& position)
{
    m_position = position;
    m_shape.setPosition(position);
}

float Particle::calculateForce(float distance, float minDistance, float radius, float maxForce)
{
    /*
    auto map = [] (float x, float inMin, float inMax, float outMin, float outMax) {
        return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
    };

    float totalForce = 0.0f;

    if(distance < minDistance)
    {
        float force = s_repulsiveForceMultiplier * std::abs(maxForce);
        force *= map(distance, 0.0f, minDistance, 1.0f, 0.0f);
        totalForce += force;
    }

    if(distance < radius)
    {
        float force = maxForce;
        force *= map(distance, 0.0f, radius, 1.0f, 0.0f);
        totalForce += force;
    }

    return totalForce;
    */

    float halfRadius = 0.5f * radius;
    float maxRepulsiveForce = -std::abs(s_repulsiveForceMultiplier * maxForce);

    if(distance == halfRadius)
        return maxForce;

    float primaryForceAscendingSlope = maxForce / (halfRadius-minDistance);
    float primaryForceDescendingSlope = -primaryForceAscendingSlope;
    float primaryForceAscendingIntercept = 0.0f - primaryForceAscendingSlope * minDistance;
    float primaryForceDescendingIntercept = maxForce - primaryForceDescendingSlope * halfRadius;

    auto primaryForce_ascendingFunction = [primaryForceAscendingSlope, primaryForceAscendingIntercept] (float x) {
        float y = primaryForceAscendingSlope * x + primaryForceAscendingIntercept;
        return primaryForceAscendingSlope >= 0.0f ? std::max(0.0f, y) : std::min(0.0f, y);
    };

    auto primaryForce_descendingFunction = [primaryForceDescendingSlope, primaryForceDescendingIntercept] (float x) {
        float y = primaryForceDescendingSlope * x + primaryForceDescendingIntercept;
        return primaryForceDescendingSlope < 0.0f ? std::max(0.0f, y) : std::min(0.0f, y);
    };

    float primaryForce = distance < halfRadius ? primaryForce_ascendingFunction(distance) : primaryForce_descendingFunction(distance);


    float repulsiveForceSlope = (0.0f - maxRepulsiveForce) / (minDistance - 0.0f);
    float repulsiveForceIntercept = maxRepulsiveForce;

    auto repulsiveForceFunction = [distance, minDistance, repulsiveForceSlope, repulsiveForceIntercept] (float x) {
        float y = repulsiveForceSlope * x + repulsiveForceIntercept;
        return distance <= minDistance && distance >= 0.0f ? y : 0.0f;
    };

    float repulsiveForce = repulsiveForceFunction(distance);

    return primaryForce + repulsiveForce;
}

uint32_t Particle::addParticleType(
    sf::Color color,
    float shapeRadius,
    float mass,
    std::vector<float> radii,
    std::vector<float> minDistances,
    std::vector<float> forces
)
{
    s_colors.push_back(color);
    s_shapeRadii.push_back(shapeRadius);
    s_masses.push_back(mass);
    s_radii.emplace_back(std::move(radii));
    s_minDistances.emplace_back(std::move(minDistances));
    s_forces.emplace_back(std::move(forces));

    return s_colors.size()-1;
}

void Particle::setRepulsiveForceMultiplier(float multiplier)
{
    s_repulsiveForceMultiplier = multiplier;
}

uint32_t Particle::getNumberOfParticleTypes()
{
    return s_colors.size();
}


}