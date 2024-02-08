#ifndef PARTICLE_LIFE_PARTICLE_SPAWNER_HPP_
#define PARTICLE_LIFE_PARTICLE_SPAWNER_HPP_

#include <memory>
#include <vector>

#include "QuadTree.hpp"
#include "Particle.hpp"

namespace particle_life {

class ParticleSpawner
{
public:
    virtual void populate(std::shared_ptr<QuadTree> pQuadTree, const sf::FloatRect& rect, uint32_t limit) = 0;
};

}

#endif