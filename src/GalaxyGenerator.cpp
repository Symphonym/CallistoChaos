#include "GalaxyGenerator.h"
#include "Utility.h"
#include <map>
#include <vector>

namespace GalaxyGenerator
{
	struct PlanetSpawn
	{
		sf::Vector2f minPos, maxPos;
		sf::IntRect subRect;
	};

	std::map<int, std::vector<PlanetSpawn>> m_planetSpawns;
	std::vector<sf::Sprite> m_planets;
	const sf::Texture *m_planetSheet;
	sf::View m_view;

	int getPlanetByChance(int random)
	{
		int i = 0;
		auto it = m_planetSpawns.begin();
		while(random > it->first)
		{
			random -= it->first;
			it++;
		}

		return it->first;
	}


	void addPlanet(int spawnChance, const sf::IntRect &subRect, const sf::Vector2f &minPos, const sf::Vector2f &maxPos)
	{
		PlanetSpawn planet;
		planet.subRect = subRect;
		planet.minPos = minPos;
		planet.maxPos = maxPos;

		m_planetSpawns[spawnChance].push_back(planet);

	}
	void setPlanetTextureSheet(const sf::Texture &texture)
	{
		m_planetSheet = &texture;
	}
	void setView(const sf::View &view)
	{
		m_view = view;
	}

	void rotate(float rotation)
	{
		m_view.rotate(rotation);
	}
	void generateGalaxy(unsigned int planets, const sf::Vector2f &planetScale)
	{
		int chanceSum = 0;
		for(auto it = m_planetSpawns.begin(); it != m_planetSpawns.end(); it++)
			chanceSum += it->first;

		for(unsigned int i = 0; i < planets; i++)
		{
			int random = jl::Math::randInt(0, chanceSum);
			int randomChance = getPlanetByChance(random);
			int randomPlanet = std::rand() % m_planetSpawns[randomChance].size();

			sf::Sprite sprite;
			sprite.setTexture(*m_planetSheet);
			sprite.setTextureRect(m_planetSpawns[randomChance][randomPlanet].subRect);
			sprite.setPosition(
				jl::Math::randDouble(m_planetSpawns[randomChance][randomPlanet].minPos.x, m_planetSpawns[randomChance][randomPlanet].maxPos.x),
				jl::Math::randDouble(m_planetSpawns[randomChance][randomPlanet].minPos.y, m_planetSpawns[randomChance][randomPlanet].maxPos.y));
			sprite.setScale(planetScale);

			m_planets.push_back(sprite);
		}
	}

	void render(sf::RenderTarget &target)
	{
		sf::View tempView(target.getView());
		target.setView(m_view);

		for(int i = 0; i < m_planets.size(); i++)
			target.draw(m_planets[i]);

		target.setView(tempView);
	}
};