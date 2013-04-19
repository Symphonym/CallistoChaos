#ifndef ENEMYWAVEMANAGER_H
#define ENEMYWAVEMANAGER_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "AssetManager.h"

class CharacterManager;
class EnemyWaveManager
{
private:

	CharacterManager *m_characters;
	jl::AssetManager *m_assets;

	// How many enemies the wave will spawn
	int m_waveEnemies;
	// How man enemies that have been spawned
	int m_waveSpawns;

	// If the wave is active or if it's a "between wave break"
	bool m_waveIsActive;

	// Spawning clock and delay
	sf::Clock m_waveSpawnTimer;
	double m_waveSpawnDelay;

	sf::Text m_waveText, m_waveInfoText, m_waveSkipText;
	sf::Color m_waveTextColor;

public:

	explicit EnemyWaveManager();
	explicit EnemyWaveManager(CharacterManager &characters, jl::AssetManager &assets);

	void update(double deltaTime);
	void render(sf::RenderTarget &target);
};

#endif