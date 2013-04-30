#include "EnemyWaveManager.h"
#include "Settings.h"
#include "CharacterManager.h"
#include "WeakEnemy.h"
#include "TileMap.h"
#include "Utility.h"
#include "GameState.h"

EnemyWaveManager::EnemyWaveManager() :
	m_characters(nullptr),
	m_assets(nullptr),
	m_waveEnemies(0),
	m_waveSpawns(0),
	m_waveIsActive(false),
	m_waveSpawnDelay(1),
	m_waveTextColor(sf::Color::White)
{
	jl::Settings::setDouble("gameEnemyMinDelay", 0.5);
	jl::Settings::setDouble("gameEnemyMaxDelay", 1.0);
	jl::Settings::setInt("gameEnemyMinCurrency", 1);
	jl::Settings::setInt("gameEnemyMaxCurrency", 3);
	jl::Settings::setInt("gameEnemyBaseSpeed", 50);
	jl::Settings::setInt("gameWaveNumber", 0);
}
EnemyWaveManager::EnemyWaveManager(CharacterManager &characters, jl::AssetManager &assets) :
	m_characters(&characters),
	m_assets(&assets),
	m_waveEnemies(0),
	m_waveSpawns(0),
	m_waveIsActive(false),
	m_waveSpawnDelay(1),
	m_waveTextColor(sf::Color::White)
{
	jl::Settings::setDouble("gameEnemyMinDelay", 0.5);
	jl::Settings::setDouble("gameEnemyMaxDelay", 1.0);
	jl::Settings::setInt("gameEnemyMinCurrency", 1);
	jl::Settings::setInt("gameEnemyMaxCurrency", 3);
	jl::Settings::setInt("gameEnemyBaseSpeed", 50);
	jl::Settings::setInt("gameWaveNumber", 0);

	m_waveText.setFont(assets.getFont("res/Minecraftia.ttf"));
	m_waveText.setCharacterSize(32);

	m_waveInfoText = sf::Text(m_waveText);
	m_waveInfoText.setCharacterSize(24);

	m_waveSkipText = sf::Text(m_waveText);
	m_waveSkipText.setCharacterSize(14);
	m_waveSkipText.setString("Press RT + LT to start the wave!");


}

void EnemyWaveManager::update(double deltaTime)
{
	if(m_waveIsActive)
	{

		if(m_waveSpawns < m_waveEnemies && m_waveSpawnTimer.getElapsedTime().asSeconds() >= m_waveSpawnDelay && m_characters->getCount() < 15)
		{
			// Get left overs
			m_waveSpawnDelay = (((double)std::rand() / (double)RAND_MAX)*5.0)/double(jl::Settings::getInt("gameWaveNumber")/2.0);
			m_waveSpawnTimer.restart();

			sf::Vector2i tileIndex(0,0);

			// Loop through outskirts of map until an available spot is found
			do
			{
				tileIndex = sf::Vector2i(0,0);

				if(std::rand() % 2 == 0)
				{
					tileIndex.x = std::rand() % m_characters->getPlayer().getTileMap().getMapSize().x;
					tileIndex.y = std::rand() % 2 == 0 ? 0 : m_characters->getPlayer().getTileMap().getMapSize().y-1;
				}
				else
				{
					tileIndex.x = std::rand() % 2 == 0 ? 0 : m_characters->getPlayer().getTileMap().getMapSize().x-1;
					tileIndex.y = std::rand() % m_characters->getPlayer().getTileMap().getMapSize().y;
				}
			}
			while(m_characters->getPlayer().getTileMap().getTile(tileIndex).isSolid() || m_characters->getPlayer().getTileMap().getTile(tileIndex).isOccupied());

			// Spawn enemy
			std::unique_ptr<WeakEnemy> enemy(new WeakEnemy(&m_characters->getPlayer().getGame(), *m_assets,tileIndex));
			enemy->setSpeed(jl::Settings::getInt("gameEnemyBaseSpeed"));
			enemy->setMoveDelay(jl::Math::randDouble(jl::Settings::getDouble("gameEnemyMinDelay"), jl::Settings::getDouble("gameEnemyMaxDelay")));

			double randChance = jl::Math::randDouble(0.0, 100.0);


			double speederPercent = (double)jl::Settings::getInt("gameWaveNumber")*0.8;
			if(speederPercent > 60)
				speederPercent = 60;
			double tankerPercent = (double)jl::Settings::getInt("gameWaveNumber")*0.2;
			if(tankerPercent > 20)
				tankerPercent = 20;

			// "Tanker" enemy types
			if(randChance <= tankerPercent)
			{
				enemy->setMaxHealth(15);
				enemy->heal(15);
				enemy->setSpeed(20);
				enemy->setMoveDelay(jl::Math::randDouble(jl::Settings::getDouble("gameEnemyMinDelay")*0.4, jl::Settings::getDouble("gameEnemyMinDelay")*0.7));
				enemy->getSprite().setColor(sf::Color(200, 150, 150));
			}
			// "Speeder" enemy types
			else if(randChance <= speederPercent)
			{
				enemy->setSpeed(jl::Math::randInt(enemy->getSpeed()*1.8, enemy->getSpeed()*2.2));
				enemy->setMoveDelay(jl::Math::randDouble(jl::Settings::getDouble("gameEnemyMinDelay")*0.4, jl::Settings::getDouble("gameEnemyMinDelay")*0.7));
				enemy->getSprite().setColor(sf::Color(150, 150, 150));
			}

			enemy->addCurrency(jl::Math::randInt(jl::Settings::getInt("gameEnemyMinCurrency"), jl::Settings::getInt("gameEnemyMaxCurrency")));

			m_characters->addCharacter(std::move(enemy));
			++m_waveSpawns;
		}

		// Wave is finished
		if(m_waveSpawns >= m_waveEnemies && m_characters->getCount() == 1)
		{
			m_waveIsActive = false;
		}
	}
	else
	{

		// Predict enemy count
		m_waveEnemies = (jl::Settings::getInt("gameWaveNumber")+1)*2;

		if(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Z) == 100)
		{
			m_waveIsActive = true;
			jl::Settings::setInt("gameWaveNumber", jl::Settings::getInt("gameWaveNumber")+1);

			m_waveSpawns = 0;

			m_waveTextColor = sf::Color::White;
			m_waveTextColor.a = 0;

			m_waveSpawnTimer.restart();
		}

		// Alpha value on the y-axis, time on the x-axis
		m_waveTextColor.a = jl::Math::lerp(m_waveTextColor.a, 255.0, deltaTime*0.5);
	}
}

void EnemyWaveManager::render(sf::RenderTarget &target)
{
	if(!m_waveIsActive)
	{
		sf::View tempView(target.getView());
		target.setView(target.getDefaultView());

		if(!m_characters->getPlayer().getGame().isPaused())
		{
			m_waveText.setColor(m_waveTextColor);
			m_waveText.setString("Wave " + jl::Util::toString(jl::Settings::getInt("gameWaveNumber")));
			m_waveText.setPosition(
				(target.getView().getSize().x * 0.5) - m_waveText.getGlobalBounds().width/2,
				(target.getView().getSize().y*0.75) - m_waveText.getGlobalBounds().height/2);

			m_waveInfoText.setColor(m_waveTextColor);
			m_waveInfoText.setString(jl::Util::toString(m_waveEnemies) + " enem" + std::string(m_waveEnemies == 1 ? "y" : "ies"));
			m_waveInfoText.setPosition(
				(m_waveText.getPosition().x + m_waveText.getGlobalBounds().width/2) - m_waveInfoText.getGlobalBounds().width/2,
				(m_waveText.getPosition().y + m_waveText.getGlobalBounds().height)+10);

			m_waveSkipText.setColor(m_waveTextColor);
			m_waveSkipText.setPosition(
				(int)(m_waveInfoText.getPosition().x + m_waveInfoText.getGlobalBounds().width/2) - (int)m_waveSkipText.getGlobalBounds().width/2,
				(int)(m_waveInfoText.getPosition().y + m_waveInfoText.getGlobalBounds().height)+10);

			target.draw(m_waveText);
			target.draw(m_waveInfoText);
			target.draw(m_waveSkipText);
		}

		target.setView(tempView);
	}
}