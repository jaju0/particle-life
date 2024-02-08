#ifndef PARTICLE_LIFE_PROGRAM_OPTIONS_HPP_
#define PARTICLE_LIFE_PROGRAM_OPTIONS_HPP_

#include <cstdint>
#include <string>
#include <boost/program_options.hpp>

namespace particle_life {

class ProgramOptions
{
public:
    ProgramOptions(const std::string& configFile);

    uint32_t getWindowWidth();
    uint32_t getWindowHeight();
    bool isFullscreen();
    const std::string& getParticleSpawnerType();
    uint32_t getNumberOfParticleTypes();
    float getForceDistMinValue();
    float getForceDistMaxValue();
    float getRadiiDistMinValue();
    float getRadiiDistMaxValue();
    float getMinDistanceDistMinValue();
    float getMinDistanceDistMaxValue();
    float getRepulsiveForceMultiplier();
    float getFrictionFactor();
    float getDampingFactor();
    uint32_t getNumberOfParticles();
    float getShapeRadius();
    float getSpeed();

private:
    boost::program_options::options_description m_description;
    boost::program_options::variables_map m_varmap;

    uint32_t m_windowWidth;
    uint32_t m_windowHeight;
    bool m_fullscreen;
    std::string m_particleSpawnerType;
    uint32_t m_numberOfParticleTypes;
    float m_forceDist_minValue;
    float m_forceDist_maxValue;
    float m_radiiDist_minValue;
    float m_radiiDist_maxValue;
    float m_minDistanceDist_minValue;
    float m_minDistanceDist_maxValue;
    float m_repulsiveForceMultiplier;
    float m_frictionFactor;
    float m_dampingFactor;
    uint32_t m_numberOfParticles;
    float m_shapeRadius;
    float m_speed;
};

}

#endif