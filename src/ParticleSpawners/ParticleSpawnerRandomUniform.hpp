#ifndef PARTICLE_LIFE_PARTICLE_SPAWNER_RANDOM_UNIFORM_HPP_
#define PARTICLE_LIFE_PARTICLE_SPAWNER_RANDOM_UNIFORM_HPP_

#include "../ParticleSpawner.hpp"

namespace particle_life {

class ParticleSpawnerRandomUniform : public ParticleSpawner
{
public:
    virtual void populate(std::shared_ptr<QuadTree> pQuadTree, const sf::FloatRect& rect, uint32_t limit) override;
};

}

#endif