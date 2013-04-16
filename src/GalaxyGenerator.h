#ifndef GALAXYGENERATOR_H
#define GALAXYGENERATOR_H

#include <SFML/Graphics.hpp>

namespace GalaxyGenerator
{

	void addPlanet(int spawnChance, const sf::IntRect &subRect, const sf::Vector2f &minPos, const sf::Vector2f &maxPos);
	void setPlanetTextureSheet(const sf::Texture &texture);
	void setView(const sf::View &view);

	void rotate(float rotation);
	void generateGalaxy(unsigned int planets, const sf::Vector2f &planetScale = sf::Vector2f(1,1));

	void render(sf::RenderTarget &target);
};

#endif