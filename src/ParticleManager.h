#ifndef PARTICLEMANAGER_H
#define PARTICLEMANAGER_H

#include <SFML/Graphics.hpp>

namespace ParticleManager
{
	struct ParticleEntity
	{
		sf::Vector2f position; // Position
		double lifeTime, aliveTime; // Life time
		double speed, initialSpeed, rotation; // Rotation
		double stopTime; // Deacceleration time
		sf::Vector2f direction; // Direction
		sf::Color particleColor; // Color
		sf::Vector2f particleSize; // Size
		bool fadeOut; // Fading
		int alpha;
		sf::IntRect subRect;
	};

	void addParticle(
		const sf::Vector2f &position, 
		double lifeTime, 
		int speed, 
		double rotation, 
		int angleDirection, 
		const sf::Color &particleColor, 
		const sf::Vector2f &particleSize,
		double stopTime = -1,
		bool fadeOut = false);
	
	void update(double deltaTime);
	void render(sf::RenderTarget &target);

	void clearParticles();
};

#endif