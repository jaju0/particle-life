#ifndef PARTICLE_LIFE_PARTICLE_SPAWNER_RANDOM_NORMAL_HPP_
#define PARTICLE_LIFE_PARTICLE_SPAWNER_RANDOM_NORMAL_HPP_

#include "../ParticleSpawner.hpp"

namespace particle_life {

class ParticleSpawnerRandomNormal : public ParticleSpawner
{
public:
    ParticleSpawnerRandomNormal(float concentration);
    virtual void populate(std::shared_ptr<QuadTree> pQuadTree, const sf::FloatRect& rect, uint32_t limit) override;

protected:
    float m_concentration;
};

}

#endif