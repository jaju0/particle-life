#include <random>

#include "ParticleSpawnerRandomUniform.hpp"

namespace particle_life {

void ParticleSpawnerRandomUniform::populate(std::shared_ptr<QuadTree> pQuadTree, const sf::FloatRect& rect, uint32_t limit)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_real_distribution<float> distWidth(rect.left, rect.left+rect.width);
    std::uniform_real_distribution<float> distHeight(rect.top, rect.top+rect.height);
    std::uniform_real_distribution<float> distParticles(0, Particle::getNumberOfParticleTypes()-1);

    for(uint32_t i = 0; i < limit; ++i)
        pQuadTree->insert(Particle(sf::Vector2f(distWidth(gen), distHeight(gen)), distParticles(gen)));
}

}