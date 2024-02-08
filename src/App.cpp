#include <random>
#include <iostream>

#include "ParticleSpawners/ParticleSpawnerRandomUniform.hpp"
#include "ParticleSpawners/ParticleSpawnerRandomNormal.hpp"
#include "App.hpp"

namespace particle_life {

App::App(ProgramOptions& programOptions)
    : m_programOptions(programOptions)
    , m_threadPool(std::thread::hardware_concurrency())
    , m_deltaTime(m_clock.restart().asSeconds())
{}

void App::start()
{
    auto windowStyle = m_programOptions.isFullscreen() ? sf::Style::Fullscreen : sf::Style::Close;
    m_pRenderWindow = std::make_shared<sf::RenderWindow>(sf::VideoMode(m_programOptions.getWindowWidth(), m_programOptions.getWindowHeight()), "Particle Life", windowStyle);

    initialize();

    while(m_pRenderWindow->isOpen())
    {
        update();
        render();
        m_deltaTime = m_clock.restart().asSeconds();
    }
}

void App::initialize()
{
    uint32_t quadTreeCapacity = 50;
    m_pQuadTree = std::make_shared<QuadTree>(sf::FloatRect(0, 0, m_programOptions.getWindowWidth(), m_programOptions.getWindowHeight()), quadTreeCapacity);
    m_pQuadTreeLastFrame = std::make_shared<QuadTree>(sf::FloatRect(0, 0, m_programOptions.getWindowWidth(), m_programOptions.getWindowHeight()), quadTreeCapacity);

    Particle::setRepulsiveForceMultiplier(m_programOptions.getRepulsiveForceMultiplier());

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> forceDist(m_programOptions.getForceDistMinValue(), m_programOptions.getForceDistMaxValue());
    std::uniform_real_distribution<float> radiiDist(m_programOptions.getRadiiDistMinValue(), m_programOptions.getRadiiDistMaxValue());
    std::uniform_real_distribution<float> minDistanceDist(m_programOptions.getMinDistanceDistMinValue(), m_programOptions.getMinDistanceDistMaxValue());
    std::uniform_int_distribution<uint32_t> intDist(0, 1);
    std::uniform_int_distribution<uint8_t> uint8Dist(0, 255);

    uint32_t numberOfTypes = m_programOptions.getNumberOfParticleTypes();
    for(uint32_t i = 0; i < numberOfTypes; ++i)
    {
        sf::Color randColor(uint8Dist(gen), uint8Dist(gen), uint8Dist(gen));
        std::vector<float> radii;
        std::vector<float> minDistances;
        std::vector<float> forces;
        for(uint32_t j = 0; j < numberOfTypes; ++j)
        {
            radii.push_back(radiiDist(gen));
            minDistances.push_back(minDistanceDist(gen));
            forces.push_back(intDist(gen) ? -forceDist(gen) : forceDist(gen));
        }

        Particle::addParticleType(randColor, m_programOptions.getShapeRadius(), 1, std::move(radii), std::move(minDistances), std::move(forces));
    }

    // create the particle swarm
    if(m_programOptions.getParticleSpawnerType() == "random_normal")
        m_pParticleSpawner = std::make_shared<ParticleSpawnerRandomNormal>(256.0f);
    else
        m_pParticleSpawner = std::make_shared<ParticleSpawnerRandomUniform>();

    m_pParticleSpawner->populate(m_pQuadTree, sf::FloatRect(0, 0, m_programOptions.getWindowWidth(), m_programOptions.getWindowHeight()), m_programOptions.getNumberOfParticles());
    *m_pQuadTreeLastFrame = *m_pQuadTree;
}

void App::update()
{
    sf::Event ev;
    while(m_pRenderWindow->pollEvent(ev))
    {
        if(ev.type == sf::Event::Closed)
            m_pRenderWindow->close();
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        m_pRenderWindow->close();

    *m_pQuadTreeLastFrame = *m_pQuadTree;
    m_pQuadTree->clear();

    std::vector<std::future<void>> futures;
    std::vector<Particle> newParticles;
    newParticles.resize(m_pQuadTreeLastFrame->size());
    uint32_t index = 0;
    for(auto& particle : *m_pQuadTreeLastFrame->getVector())
    {
        futures.emplace_back(this->m_threadPool.addTask([&, this, myIndex=index] {
            Particle particleCopy(particle);
            float radius = m_programOptions.getRadiiDistMaxValue();
            auto neighbours = this->m_pQuadTreeLastFrame->query(particleCopy.getPosition(), radius);

            particleCopy.update(
                m_deltaTime,
                m_programOptions.getSpeed(),
                m_programOptions.getFrictionFactor(),
                m_programOptions.getDampingFactor(),
                neighbours, &particle, {
                static_cast<float>(m_programOptions.getWindowWidth()), 
                static_cast<float>(m_programOptions.getWindowHeight())
            });

            newParticles.at(myIndex) = std::move(particleCopy);
        }));

        index++;
    }

    for(auto& future : futures)
        future.wait();

    for(auto& particle : newParticles)
        this->m_pQuadTree->insert(std::move(particle));
}

void App::render()
{
    m_pRenderWindow->clear(sf::Color::Black);

    m_pQuadTree->forEach([this] (Particle* pParticle) {
        this->m_pRenderWindow->draw(*pParticle);
    });

    m_pRenderWindow->display();
}

}
