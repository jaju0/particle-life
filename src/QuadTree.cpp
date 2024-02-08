#include <iostream>
#include "mathhelper.hpp"
#include "QuadTree.hpp"

namespace particle_life {


QuadTree::QuadTree(const sf::FloatRect& boundary, uint32_t capacity)
    : m_boundary(boundary)
    , m_rootBoundary(boundary)
    , m_isDivided(false)
    , m_capacity(capacity)
    , m_pVector(std::make_shared<std::vector<Particle>>())
{
    m_particles.reserve(m_capacity);
}

QuadTree::QuadTree(const sf::FloatRect& boundary, uint32_t capacity, const sf::FloatRect& rootBoundary, std::shared_ptr<std::vector<Particle>> pVector)
    : m_boundary(boundary)
    , m_rootBoundary(rootBoundary)
    , m_isDivided(false)
    , m_capacity(capacity)
    , m_pVector(pVector)
{
    m_particles.reserve(m_capacity);
}

QuadTree& QuadTree::operator=(QuadTree& other)
{
    clear();
    m_boundary = other.m_boundary;
    m_rootBoundary = other.m_rootBoundary;
    m_capacity = other.m_capacity;
    m_particles.reserve(m_capacity);

    for(auto& particle : *other.m_pVector)
        this->insert(Particle(particle));

    return *this;
}

bool QuadTree::insert(Particle&& particle)
{
    auto& particlePos = particle.getPosition();

    if(!rectContainsOrTouches(m_boundary, particlePos))
        return false;

    if(m_particles.size() < m_capacity)
    {
        m_pVector->emplace_back(particle);
        m_particles.push_back(&m_pVector->back());
        return true;
    }

    if(!m_isDivided)
    {
        float halfBoundaryWidth = m_boundary.width / 2.0f;
        float halfBoundaryHeight = m_boundary.height / 2.0f;
        m_pTopLeft = std::make_unique<QuadTree>(sf::FloatRect(m_boundary.left, m_boundary.top, halfBoundaryWidth, halfBoundaryHeight), m_capacity, m_rootBoundary, m_pVector);
        m_pTopRight = std::make_unique<QuadTree>(sf::FloatRect(m_boundary.left + halfBoundaryWidth, m_boundary.top, halfBoundaryWidth, halfBoundaryHeight), m_capacity, m_rootBoundary, m_pVector);
        m_pBottomLeft = std::make_unique<QuadTree>(sf::FloatRect(m_boundary.left, m_boundary.top + halfBoundaryHeight, halfBoundaryWidth, halfBoundaryHeight), m_capacity, m_rootBoundary, m_pVector);
        m_pBottomRight = std::make_unique<QuadTree>(sf::FloatRect(m_boundary.left + halfBoundaryWidth, m_boundary.top + halfBoundaryHeight, halfBoundaryWidth, halfBoundaryHeight), m_capacity, m_rootBoundary, m_pVector);
        m_isDivided = true;
    }

    return (
        m_pTopLeft->insert(std::move(particle)) ||
        m_pTopRight->insert(std::move(particle)) ||
        m_pBottomLeft->insert(std::move(particle)) || 
        m_pBottomRight->insert(std::move(particle))
    );
}

std::vector<Particle*> QuadTree::query(const sf::FloatRect& rect)
{
    std::vector<Particle*> foundParticles;

    sf::FloatRect verticalMirroredBoundary;
    sf::FloatRect horizontalMirroredBoundary;
    sf::FloatRect completeMirroredBoundary;
    std::tie(verticalMirroredBoundary, horizontalMirroredBoundary, completeMirroredBoundary) = getMirroredBoundaries();

    if(
        !m_boundary.intersects(rect) &&
        !verticalMirroredBoundary.intersects(rect) &&
        !horizontalMirroredBoundary.intersects(rect) &&
        !completeMirroredBoundary.intersects(rect)
    )
        return foundParticles;

    for(auto pParticle : m_particles)
    {
        sf::Vector2f verticalMirroredPosition;
        sf::Vector2f horizontalMirroredPosition;
        sf::Vector2f completeMirroredPosition;
        std::tie(verticalMirroredPosition, horizontalMirroredPosition, completeMirroredPosition) = getMirroredPositions(pParticle->getPosition());

        if(
            rect.contains(pParticle->getPosition()) ||
            rect.contains(verticalMirroredPosition) ||
            rect.contains(horizontalMirroredPosition) ||
            rect.contains(completeMirroredPosition)
        )
            foundParticles.push_back(pParticle);
    }

    if(m_isDivided)
    {
        m_pTopLeft->query(rect, foundParticles);
        m_pTopRight->query(rect, foundParticles);
        m_pBottomLeft->query(rect, foundParticles);
        m_pBottomRight->query(rect, foundParticles);
    }

    return foundParticles;
}

void QuadTree::query(const sf::FloatRect& rect, std::vector<Particle*>& foundParticles)
{
    sf::FloatRect verticalMirroredBoundary;
    sf::FloatRect horizontalMirroredBoundary;
    sf::FloatRect completeMirroredBoundary;
    std::tie(verticalMirroredBoundary, horizontalMirroredBoundary, completeMirroredBoundary) = getMirroredBoundaries();

    if(
        !m_boundary.intersects(rect) &&
        !verticalMirroredBoundary.intersects(rect) &&
        !horizontalMirroredBoundary.intersects(rect) &&
        !completeMirroredBoundary.intersects(rect)
    )
        return;

    for(auto pParticle : m_particles)
    {
        sf::Vector2f verticalMirroredPosition;
        sf::Vector2f horizontalMirroredPosition;
        sf::Vector2f completeMirroredPosition;
        std::tie(verticalMirroredPosition, horizontalMirroredPosition, completeMirroredPosition) = getMirroredPositions(pParticle->getPosition());

        if(
            rect.contains(pParticle->getPosition()) ||
            rect.contains(verticalMirroredPosition) ||
            rect.contains(horizontalMirroredPosition) ||
            rect.contains(completeMirroredPosition)
        )
            foundParticles.push_back(pParticle);
    }

    if(m_isDivided)
    {
        m_pTopLeft->query(rect, foundParticles);
        m_pTopRight->query(rect, foundParticles);
        m_pBottomLeft->query(rect, foundParticles);
        m_pBottomRight->query(rect, foundParticles);
    }
}

std::vector<Particle*> QuadTree::query(sf::Vector2f center, float radius)
{
    std::vector<Particle*> foundParticles;

    sf::FloatRect verticalMirroredBoundary;
    sf::FloatRect horizontalMirroredBoundary;
    sf::FloatRect completeMirroredBoundary;
    std::tie(verticalMirroredBoundary, horizontalMirroredBoundary, completeMirroredBoundary) = getMirroredBoundaries();

    if(
        !rectangleIntersectsCircle(center, radius, m_boundary) &&
        !rectangleIntersectsCircle(center, radius, verticalMirroredBoundary) &&
        !rectangleIntersectsCircle(center, radius, horizontalMirroredBoundary) &&
        !rectangleIntersectsCircle(center, radius, completeMirroredBoundary)
    )
        return foundParticles;

    for(auto pParticle : m_particles)
    {
        sf::Vector2f verticalMirroredPosition;
        sf::Vector2f horizontalMirroredPosition;
        sf::Vector2f completeMirroredPosition;
        std::tie(verticalMirroredPosition, horizontalMirroredPosition, completeMirroredPosition) = getMirroredPositions(pParticle->getPosition());

        if(
            isPointInCircle(center, radius, pParticle->getPosition()) ||
            isPointInCircle(center, radius, verticalMirroredPosition) ||
            isPointInCircle(center, radius, horizontalMirroredPosition) ||
            isPointInCircle(center, radius, completeMirroredPosition)
        )
            foundParticles.push_back(pParticle);
    }

    if(m_isDivided)
    {
        m_pTopLeft->query(center, radius, foundParticles);
        m_pTopRight->query(center, radius, foundParticles);
        m_pBottomLeft->query(center, radius, foundParticles);
        m_pBottomRight->query(center, radius, foundParticles);
    }

    return foundParticles;
}

void QuadTree::query(sf::Vector2f center, float radius, std::vector<Particle*>& foundParticles)
{
    sf::FloatRect verticalMirroredBoundary;
    sf::FloatRect horizontalMirroredBoundary;
    sf::FloatRect completeMirroredBoundary;
    std::tie(verticalMirroredBoundary, horizontalMirroredBoundary, completeMirroredBoundary) = getMirroredBoundaries();

    if(
        !rectangleIntersectsCircle(center, radius, m_boundary) &&
        !rectangleIntersectsCircle(center, radius, verticalMirroredBoundary) &&
        !rectangleIntersectsCircle(center, radius, horizontalMirroredBoundary) &&
        !rectangleIntersectsCircle(center, radius, completeMirroredBoundary)
    )
        return;

    for(auto pParticle : m_particles)
    {
        sf::Vector2f verticalMirroredPosition;
        sf::Vector2f horizontalMirroredPosition;
        sf::Vector2f completeMirroredPosition;
        std::tie(verticalMirroredPosition, horizontalMirroredPosition, completeMirroredPosition) = getMirroredPositions(pParticle->getPosition());

        if(
            isPointInCircle(center, radius, pParticle->getPosition()) ||
            isPointInCircle(center, radius, verticalMirroredPosition) ||
            isPointInCircle(center, radius, horizontalMirroredPosition) ||
            isPointInCircle(center, radius, completeMirroredPosition)
        )
            foundParticles.push_back(pParticle);
    }

    if(m_isDivided)
    {
        m_pTopLeft->query(center, radius, foundParticles);
        m_pTopRight->query(center, radius, foundParticles);
        m_pBottomLeft->query(center, radius, foundParticles);
        m_pBottomRight->query(center, radius, foundParticles);
    }
}

void QuadTree::forEach(std::function<void(Particle*)> func)
{
    for(auto pParticle : m_particles)
        func(pParticle);
    
    if(m_isDivided)
    {
        m_pTopLeft->forEach(func);
        m_pTopRight->forEach(func);
        m_pBottomLeft->forEach(func);
        m_pBottomRight->forEach(func);
    }
}

void QuadTree::clear()
{
    m_particles.clear();
    m_pTopLeft = nullptr;
    m_pTopRight = nullptr;
    m_pBottomLeft = nullptr;
    m_pBottomRight = nullptr;
    m_isDivided = false;

    if(m_pVector)
        m_pVector->clear();
}

uint32_t QuadTree::size()
{
    uint32_t size = m_particles.size();

    if(m_isDivided)
    {
        size += m_pTopLeft->size();
        size += m_pTopRight->size();
        size += m_pBottomLeft->size();
        size += m_pBottomRight->size();
    }

    return size;
}

std::shared_ptr<std::vector<Particle>> QuadTree::getVector()
{
    return m_pVector;
}

std::tuple<sf::FloatRect, sf::FloatRect, sf::FloatRect> QuadTree::getMirroredBoundaries()
{
    sf::FloatRect verticalMirroredBoundary(m_boundary);
    if(m_boundary.top < m_rootBoundary.top + 0.5f * m_rootBoundary.height)
        verticalMirroredBoundary.top += m_rootBoundary.height;
    else
        verticalMirroredBoundary.top -= m_rootBoundary.height;

    sf::FloatRect horizontalMirroredBoundary(m_boundary);
    if(m_boundary.left < m_rootBoundary.left + 0.5f * m_rootBoundary.width)
        horizontalMirroredBoundary.left += m_rootBoundary.width;
    else
        horizontalMirroredBoundary.left -= m_rootBoundary.width;

    sf::FloatRect completeMirroredBoundary(m_boundary);
    completeMirroredBoundary.top = verticalMirroredBoundary.top;
    completeMirroredBoundary.left = horizontalMirroredBoundary.left;

    return { verticalMirroredBoundary, horizontalMirroredBoundary, completeMirroredBoundary };
}

std::tuple<sf::Vector2f, sf::Vector2f, sf::Vector2f> QuadTree::getMirroredPositions(const sf::Vector2f& position)
{
    sf::Vector2f verticalMirroredPosition(position);
    if(position.y < m_rootBoundary.top + 0.5f * m_rootBoundary.height)
        verticalMirroredPosition.y += m_rootBoundary.height;
    else
        verticalMirroredPosition.y -= m_rootBoundary.height;

    sf::Vector2f horizontalMirroredPosition(position);
    if(position.x < m_rootBoundary.left + 0.5f * m_rootBoundary.width)
        horizontalMirroredPosition.x += m_rootBoundary.width;
    else
        horizontalMirroredPosition.x -= m_rootBoundary.width;

    sf::Vector2f completeMirroredPosition(position);
    completeMirroredPosition.x = horizontalMirroredPosition.x;
    completeMirroredPosition.y = verticalMirroredPosition.y;

    return {verticalMirroredPosition, horizontalMirroredPosition, completeMirroredPosition};
}


}