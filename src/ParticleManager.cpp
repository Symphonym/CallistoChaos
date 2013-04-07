#include "ParticleManager.h"
#include <vector>
#include "Utility.h"

namespace ParticleManager
{
	std::vector<ParticleEntity> m_particles;
	sf::RectangleShape m_particle;

	void addParticle(
		const sf::Vector2f &position, 
		double lifeTime, 
		double speed, 
		double rotation, 
		int angleDirection, 
		const sf::Color &particleColor, 
		const sf::Vector2f &particleSize, 
		double stopTime,
		bool fadeOut)
	{
		ParticleEntity particle;
		particle.position = position;
		particle.lifeTime = lifeTime;
		particle.aliveTime = 0.0;
		particle.speed = speed;
		particle.initialSpeed = speed;
		particle.rotation = rotation;
		particle.stopTime = stopTime;
		particle.direction = sf::Vector2f(std::sin(jl::Math::degToRad<double>(angleDirection)), std::cos(jl::Math::degToRad<double>(angleDirection)));
		particle.particleColor = particleColor;
		particle.particleSize = particleSize;
		particle.fadeOut = fadeOut;
		particle.alpha = 255;
		m_particles.push_back(particle);
	}

	void update(double deltaTime)
	{
		for(int i = 0; i < m_particles.size(); i++)
		{
			if(m_particles[i].aliveTime >= m_particles[i].lifeTime)
			{
				m_particles.erase(m_particles.begin() + i);
				continue;
			}
			else
			{
				m_particles[i].aliveTime += deltaTime;
				m_particles[i].position.x += m_particles[i].direction.x*m_particles[i].speed*deltaTime;
				m_particles[i].position.y += m_particles[i].direction.y*m_particles[i].speed*deltaTime;

				if(m_particles[i].fadeOut)
					m_particles[i].alpha = jl::Vec::lerp(sf::Vector2f(0, 255), sf::Vector2f(m_particles[i].lifeTime, 0), m_particles[i].aliveTime/m_particles[i].lifeTime).y;

				if(m_particles[i].stopTime > 0 && m_particles[i].aliveTime < m_particles[i].stopTime)
					m_particles[i].speed = jl::Vec::lerp(sf::Vector2f(0, m_particles[i].initialSpeed), sf::Vector2f(m_particles[i].stopTime, 0),  m_particles[i].aliveTime/m_particles[i].stopTime).y;
			}
		}
	}
	void render(sf::RenderTarget &target)
	{
		for(int i = 0; i < m_particles.size(); i++)
		{
			m_particle.setSize(m_particles[i].particleSize);
			sf::Color particleColor = m_particles[i].particleColor;
			particleColor.a = m_particles[i].alpha;
			m_particle.setFillColor(particleColor);
			m_particle.setOrigin(m_particle.getGlobalBounds().width/2, m_particle.getGlobalBounds().height/2);
			m_particle.setRotation(m_particles[i].rotation);
			m_particle.setPosition(m_particles[i].position);

			target.draw(m_particle);
		}
	}
};