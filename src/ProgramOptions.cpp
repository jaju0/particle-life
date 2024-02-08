#include <SFML/Graphics.hpp>
#include "ProgramOptions.hpp"

namespace particle_life {

ProgramOptions::ProgramOptions(const std::string& configFile)
    : m_description("particle")
    , m_windowWidth(0)
    , m_windowHeight(0)
    , m_fullscreen(false)
    , m_numberOfParticleTypes(0)
    , m_forceDist_minValue(0.0f)
    , m_forceDist_maxValue(0.0f)
    , m_radiiDist_minValue(0.0f)
    , m_radiiDist_maxValue(0.0f)
    , m_minDistanceDist_minValue(0.0f)
    , m_minDistanceDist_maxValue(0.0f)
    , m_repulsiveForceMultiplier(0.0f)
    , m_frictionFactor(0.0f)
    , m_dampingFactor(0.0f)
    , m_numberOfParticles(0)
    , m_speed(0.0f)
{
    m_description.add_options()
        ("window.width", boost::program_options::value<uint32_t>(), "window width")
        ("window.height", boost::program_options::value<uint32_t>(), "window height")
        ("window.fullscreen", boost::program_options::value<bool>()->default_value(true), "fullscreen")
        ("particle.particle_spawner_type", boost::program_options::value<std::string>()->default_value("random_normal"), "type of particle spawner")
        ("particle.number_of_types", boost::program_options::value<uint32_t>()->default_value(10), "number of particle types")
        ("particle.force_distribution_min_value", boost::program_options::value<float>()->default_value(0.3f), "force distribution min. value")
        ("particle.force_distribution_max_value", boost::program_options::value<float>()->default_value(1.0f), "force distribution max. value")
        ("particle.radii_distribution_min_value", boost::program_options::value<float>()->default_value(70.0f), "radii distribution min. value")
        ("particle.radii_distribution_max_value", boost::program_options::value<float>()->default_value(250.0f), "radii distribution max. value")
        ("particle.minDistance_distribution_min_value", boost::program_options::value<float>()->default_value(30.0f), "minDistance distribution min. value")
        ("particle.minDistance_distribution_max_value", boost::program_options::value<float>()->default_value(50.0f), "minDistance distribution max. value")
        ("particle.repulsive_force_multiplier", boost::program_options::value<float>()->default_value(16.0f), "repulsive force multiplier")
        ("particle.friction_factor", boost::program_options::value<float>()->default_value(0.85f), "friction factor")
        ("particle.damping_factor", boost::program_options::value<float>()->default_value(0.05f), "damping factor")
        ("particle.number_of_particles", boost::program_options::value<uint32_t>()->default_value(3000), "number of particles to spawn")
        ("particle.shape_radius", boost::program_options::value<float>()->default_value(2.0f), "shape radius")
        ("particle.speed", boost::program_options::value<float>()->default_value(0.5f), "speed of particle movement")
    ;

    boost::program_options::store(boost::program_options::parse_config_file(configFile.c_str(), m_description), m_varmap);
    boost::program_options::notify(m_varmap);

    if(m_varmap.count("window.width") && m_varmap.count("window.height"))
    {
        m_windowWidth = m_varmap.at("window.width").as<uint32_t>();
        m_windowHeight = m_varmap.at("window.height").as<uint32_t>();
    }
    else
    {
        auto desktopMode = sf::VideoMode::getDesktopMode();
        m_windowWidth = desktopMode.width;
        m_windowHeight = desktopMode.height;
    }

    if(m_varmap.count("window.fullscreen"))
        m_fullscreen = m_varmap.at("window.fullscreen").as<bool>();

    if(m_varmap.count("particle.particle_spawner_type"))
        m_particleSpawnerType = m_varmap.at("particle.particle_spawner_type").as<std::string>();
    if(m_varmap.count("particle.number_of_types"))
        m_numberOfParticleTypes = m_varmap.at("particle.number_of_types").as<uint32_t>();
    if(m_varmap.count("particle.force_distribution_min_value"))
        m_forceDist_minValue = m_varmap.at("particle.force_distribution_min_value").as<float>();
    if(m_varmap.count("particle.force_distribution_max_value"))
        m_forceDist_maxValue = m_varmap.at("particle.force_distribution_max_value").as<float>();
    if(m_varmap.count("particle.radii_distribution_min_value"))
        m_radiiDist_minValue = m_varmap.at("particle.radii_distribution_min_value").as<float>();
    if(m_varmap.count("particle.radii_distribution_max_value"))
        m_radiiDist_maxValue = m_varmap.at("particle.radii_distribution_max_value").as<float>();
    if(m_varmap.count("particle.minDistance_distribution_min_value"))
        m_minDistanceDist_minValue = m_varmap.at("particle.minDistance_distribution_min_value").as<float>();
    if(m_varmap.count("particle.minDistance_distribution_max_value"))
        m_minDistanceDist_maxValue = m_varmap.at("particle.minDistance_distribution_max_value").as<float>();
    if(m_varmap.count("particle.repulsive_force_multiplier"))
        m_repulsiveForceMultiplier = m_varmap.at("particle.repulsive_force_multiplier").as<float>();
    if(m_varmap.count("particle.friction_factor"))
        m_frictionFactor = m_varmap.at("particle.friction_factor").as<float>();
    if(m_varmap.count("particle.damping_factor"))
        m_dampingFactor = m_varmap.at("particle.damping_factor").as<float>();
    if(m_varmap.count("particle.number_of_particles"))
        m_numberOfParticles = m_varmap.at("particle.number_of_particles").as<uint32_t>();
    if(m_varmap.count("particle.shape_radius"))
        m_shapeRadius = m_varmap.at("particle.shape_radius").as<float>();
    if(m_varmap.count("particle.speed"))
        m_speed = m_varmap.at("particle.speed").as<float>();
}

uint32_t ProgramOptions::getWindowWidth()
{
    return m_windowWidth;
}

uint32_t ProgramOptions::getWindowHeight()
{
    return m_windowHeight;
}

bool ProgramOptions::isFullscreen()
{
    return m_fullscreen;
}

const std::string& ProgramOptions::getParticleSpawnerType()
{
    return m_particleSpawnerType;
}

uint32_t ProgramOptions::getNumberOfParticleTypes()
{
 return m_numberOfParticleTypes;
}

float ProgramOptions::getForceDistMinValue()
{
    return m_forceDist_minValue;
}

float ProgramOptions::getForceDistMaxValue()
{
    return m_forceDist_maxValue;
}

float ProgramOptions::getRadiiDistMinValue()
{
    return m_radiiDist_minValue;
}

float ProgramOptions::getRadiiDistMaxValue()
{
    return m_radiiDist_maxValue;
}

float ProgramOptions::getMinDistanceDistMinValue()
{
    return m_minDistanceDist_minValue;
}

float ProgramOptions::getMinDistanceDistMaxValue()
{
    return m_minDistanceDist_maxValue;
}

float ProgramOptions::getRepulsiveForceMultiplier()
{
    return m_repulsiveForceMultiplier;
}

float ProgramOptions::getFrictionFactor()
{
    return m_frictionFactor;
}

float ProgramOptions::getDampingFactor()
{
    return m_dampingFactor;
}


uint32_t ProgramOptions::getNumberOfParticles()
{
    return m_numberOfParticles;
}

float ProgramOptions::getShapeRadius()
{
    return m_shapeRadius;
}

float ProgramOptions::getSpeed()
{
    return m_speed;
}


}