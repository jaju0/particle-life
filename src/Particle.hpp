#ifndef PARTICLE_LIFE_PARTICLE_HPP_
#define PARTICLE_LIFE_PARTICLE_HPP_

#include <SFML/Graphics.hpp>

namespace particle_life {

class Particle : public sf::Drawable
{
public:
    Particle();
    Particle(const sf::Vector2f& position, uint32_t typeId);
    void update(float deltaTime, float speed, float frictionFactor, float dampingFactor, std::vector<Particle*>& neighbours, Particle* pParticleLastFrame, sf::Vector2f planeSize);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    uint32_t getTypeId();
    const sf::Vector2f& getPosition() const;


    static uint32_t addParticleType(
        sf::Color color,
        float shapeRadius,
        float mass,
        std::vector<float> radii,
        std::vector<float> minDistances,
        std::vector<float> forces
    );

    static void setRepulsiveForceMultiplier(float multiplier);
    static uint32_t getNumberOfParticleTypes();

private:
    uint32_t m_typeId;
    sf::CircleShape m_shape;
    sf::Vector2f m_position;
    float m_velocity;

    void setPosition(const sf::Vector2f& position);
    float calculateForce(float distance, float minDistance, float radius, float maxForce);


    static std::vector<sf::Color> s_colors;
    static std::vector<float> s_shapeRadii;
    static std::vector<float> s_masses;
    static std::vector<std::vector<float>> s_radii;
    static std::vector<std::vector<float>> s_minDistances;
    static std::vector<std::vector<float>> s_forces;
    static float s_repulsiveForceMultiplier;
};

}

#endif