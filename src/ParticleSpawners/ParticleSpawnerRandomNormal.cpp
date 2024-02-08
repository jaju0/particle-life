#include <random>

#include "ParticleSpawnerRandomNormal.hpp"

namespace particle_life {

ParticleSpawnerRandomNormal::ParticleSpawnerRandomNormal(float concentration)
    : m_concentration(concentration)
{}

void ParticleSpawnerRandomNormal::populate(std::shared_ptr<QuadTree> pQuadTree, const sf::FloatRect& rect, uint32_t limit)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    sf::IntRect intRect(
        static_cast<int32_t>(rect.left),
        static_cast<int32_t>(rect.top),
        static_cast<int32_t>(rect.width),
        static_cast<int32_t>(rect.height)
    );

    std::normal_distribution<float> distWidth(rect.left + rect.width / 2.0f, rect.width / m_concentration);
    std::normal_distribution<float> distHeight(rect.top + rect.height / 2.0f, rect.height / m_concentration);
    std::uniform_int_distribution<int32_t> distParticles(0, Particle::getNumberOfParticleTypes()-1);

    for(uint32_t i = 0; i < limit; ++i)
        pQuadTree->insert(Particle(sf::Vector2f(distWidth(gen), distHeight(gen)), distParticles(gen)));
}


}