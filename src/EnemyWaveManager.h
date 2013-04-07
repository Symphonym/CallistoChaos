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

	int m_waveNumber;

	// How many enemies the wave will spawn
	int m_waveEnemies;
	// How man enemies that have been spawned
	int m_waveSpawns;

	// If the wave is active or if it's a "between wave break"
	bool m_waveIsActive;

	// How many seconds that's left on the break
	int m_waveBreakLeft;
	int m_waveBreakTime;
	sf::Clock m_waveBreakTimer;

	// Spawning clock and delay
	sf::Clock m_waveSpawnTimer;
	double m_waveSpawnDelay;

	sf::Text m_waveText, m_waveInfoText;
	sf::Color m_waveTextColor;

public:

	explicit EnemyWaveManager();
	explicit EnemyWaveManager(CharacterManager &characters, jl::AssetManager &assets);

	void update(double deltaTime);
	void render(sf::RenderTarget &target);
};

#endif