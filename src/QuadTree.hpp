#ifndef PARTICLE_LIFE_QUADTREE_HPP_
#define PARTICLE_LIFE_QUADTREE_HPP_

#include <memory>
#include <vector>
#include <tuple>
#include <functional>
#include <SFML/Graphics.hpp>

#include "Particle.hpp"

namespace particle_life {

class QuadTree
{
public:
    QuadTree(const sf::FloatRect& boundary, uint32_t capacity);
    QuadTree(const sf::FloatRect& boundary, uint32_t capacity, const sf::FloatRect& rootBoundary, std::shared_ptr<std::vector<Particle>> pVector);

    QuadTree& operator=(QuadTree& other);

    bool insert(Particle&& particle);
    std::vector<Particle*> query(const sf::FloatRect& rect);
    void query(const sf::FloatRect& rect, std::vector<Particle*>& foundParticles);
    std::vector<Particle*> query(sf::Vector2f center, float radius);
    void query(sf::Vector2f center, float radius, std::vector<Particle*>& foundParticles);
    void forEach(std::function<void(Particle*)> func);
    void clear();
    uint32_t size();
    std::shared_ptr<std::vector<Particle>> getVector();

private:
    std::tuple<sf::FloatRect, sf::FloatRect, sf::FloatRect> getMirroredBoundaries();
    std::tuple<sf::Vector2f, sf::Vector2f, sf::Vector2f> getMirroredPositions(const sf::Vector2f& position);

    sf::FloatRect m_boundary;
    sf::FloatRect m_rootBoundary;

    bool m_isDivided;
    std::unique_ptr<QuadTree> m_pTopLeft;
    std::unique_ptr<QuadTree> m_pTopRight;
    std::unique_ptr<QuadTree> m_pBottomLeft;
    std::unique_ptr<QuadTree> m_pBottomRight;

    uint32_t m_capacity;
    std::vector<Particle*> m_particles;

    std::shared_ptr<std::vector<Particle>> m_pVector;
};

}

#endif