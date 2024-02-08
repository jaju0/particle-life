#ifndef PARTICLE_LIFE_APP_HPP_
#define PARTICLE_LIFE_APP_HPP_

#include "ProgramOptions.hpp"
#include "ThreadPool.hpp"
#include "QuadTree.hpp"
#include "ParticleSpawner.hpp"

#include <memory>
#include <SFML/Graphics.hpp>

namespace particle_life {

class App
{
public:
    App(ProgramOptions& programOptions);
    void start();

private:
    void initialize();
    void update();
    void render();

    ProgramOptions& m_programOptions;

    ThreadPool m_threadPool;
    std::shared_ptr<sf::RenderWindow> m_pRenderWindow;
    sf::Clock m_clock;
    float m_deltaTime;

    std::shared_ptr<ParticleSpawner> m_pParticleSpawner;
    std::shared_ptr<QuadTree> m_pQuadTreeLastFrame;
    std::shared_ptr<QuadTree> m_pQuadTree;
};

}

#endif