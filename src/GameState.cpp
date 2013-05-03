#include "GameState.h"
#include "Player.h"
#include "MessageLog.h"
#include "TileOptionActions.h"
#include "WeakEnemy.h"
#include "Utility.h"
#include "ParticleManager.h"
#include "SettingsConsole.h"
#include "GalaxyGenerator.h"
#include <fstream>
#include "SoundManager.h"

GameState::GameState(jl::Engine *engine) : 
	jl::State(engine),
	m_updatedHighscore(false)
{
	std::vector<std::vector<int>> gameLevel = {
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,3,4,4,6,4,3,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,3,9,8,8,11,3,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,3,8,8,8,8,5,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,3,10,8,8,8,3,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
	// Throw in some random flowers at certain intervalls
	for(std::size_t y = 0; y < 20; y++)
		for(std::size_t x = 0; x < 24; x++)
			if(!(x > 9 && x < 16 && y > 7 && y < 14)) // Set an area not have bushes generated
				gameLevel[y][x] = std::rand() % 100 < 20 ? (std::rand() % 100 < 40 ? 1 : 2) : 0;

	// Specify tilemap data
	m_tileMap.setData(
		getEngine()->getAssets().getTexture("res/tiles.png"),
		sf::Vector2i(24, 20),
		16);


	m_tileMap.addType(0, sf::IntRect(0,0, 16, 16)); // Ground
	m_tileMap.addType(1, sf::IntRect(16,0, 16, 16)); // Flower
	m_tileMap.addDType(2, sf::IntRect(32,0, 16, 16), sf::IntRect(16,48, 16, 16), 20, true, true); // Bush
	m_tileMap.addType(3, sf::IntRect(0, 16, 16, 16), true, true); // Wall side
	m_tileMap.addType(4, sf::IntRect(16, 16, 16, 16), true, true); // Wall top
	m_tileMap.addDType(5, sf::IntRect(32, 16, 16, 16), sf::IntRect(16, 64, 16, 16), 15, false, true); // Window
	m_tileMap.addDType(6, sf::IntRect(0, 32, 16, 16), sf::IntRect(48, 32, 16, 16), 20, true, true); // Door closed
	m_tileMap.addType(7, sf::IntRect(16, 32, 16, 16)); // Door open
	m_tileMap.addType(8, sf::IntRect(32, 32, 16, 16)); // Floor
	m_tileMap.addType(9, sf::IntRect(48, 48, 16, 16), false, true); // Ammo box
	m_tileMap.addType(10, sf::IntRect(0, 48, 16, 16), false, true); // Bed
	m_tileMap.addType(11, sf::IntRect(0, 64, 16, 16), false, true); // Workbench
	m_tileMap.loadFromData(gameLevel);

	// Set camera data
	reloadView();

	SettingsConsole::setFontData(getEngine()->getAssets().getFont("res/Minecraftia.ttf"), 24);

	// Load assets
	m_tileOptions.loadAssets(getEngine()->getAssets());
	MessageLog::loadAssets(getEngine()->getAssets());

	m_backgroundPlanet.setTexture(getEngine()->getAssets().getTexture("res/planet.png"));
	m_backgroundPlanet.setScale(
		1000.0 / m_backgroundPlanet.getTexture()->getSize().x,
		1000.0 / m_backgroundPlanet.getTexture()->getSize().y);
	m_backgroundPlanet.setPosition(
		m_view.getCenter().x - (m_backgroundPlanet.getGlobalBounds().width/2),
		m_view.getCenter().x - (m_backgroundPlanet.getGlobalBounds().height/2));

	// Load character manager
	std::unique_ptr<Player> player(new Player(this, getEngine()->getAssets(), sf::Vector2i(13,11)));
	// Set player to be used for the Tile options
	m_tileOptions.provideCharacter(player.get());
	m_loot = LootManager(getEngine()->getAssets());
	m_loot.providePlayer(player.get());
	m_workbench = Workbench(player.get(), getEngine()->getAssets());
	m_bedControl = BedControl(player.get());
	m_enemyWaves = EnemyWaveManager(m_characters, getEngine()->getAssets());

	// Give player to manager
	m_characters.addPlayer(std::move(player));

	jl::SoundManager::addSound("res/damage.wav");
	jl::SoundManager::addSound("res/death.wav");
	jl::SoundManager::addSound("res/actionconfirmed.wav");
	jl::SoundManager::getSound("res/actionconfirmed.wav").setVolume(60);
	jl::SoundManager::addSound("res/actiondenied.wav");

	// Add interactive options for tiles
	// Flower
	m_tileOptions.addOption(1, "Look at", std::bind(&MessageLog::addMessage, "A small flower"));
	m_tileOptions.addOption(1, "Examine", std::bind(&MessageLog::addMessage, "It's very small, I can easily walk over it"));
	m_tileOptions.addOption(1, "Water", std::bind(&MessageLog::addMessage, "You spit graciously on the flower"));
	m_tileOptions.addOption(1, "Durability", TileOptionActions::examineHealth);

	// Bush
	m_tileOptions.addOption(2, "Look at", std::bind(&MessageLog::addMessage, "Looks like a bush"));
	m_tileOptions.addOption(2, "Examine", std::bind(&MessageLog::addMessage, "Rock solid, no way I'm getting through this bush"));
	m_tileOptions.addOption(2, "Talk to", std::bind(&MessageLog::addMessage, "Sup, bush?"));
	m_tileOptions.addOption(2, "Think", std::bind(&MessageLog::addMessage, "It's a bush alright"));
	m_tileOptions.addOption(2, "Durability", TileOptionActions::examineHealth);

	// Closed door
	m_tileOptions.addOption(6, "Open", TileOptionActions::openDoor);
	m_tileOptions.addOption(6, "Repair", TileOptionActions::repair);
	m_tileOptions.addOption(6, "Look At", std::bind(&MessageLog::addMessage, "A solid wooden door"));
	m_tileOptions.addOption(6, "Examine", std::bind(&MessageLog::addMessage, "I could probably smash it open after a few hits"));
	m_tileOptions.addOption(6, "Durability", TileOptionActions::examineHealth);

	// Open door
	m_tileOptions.addOption(7, "Close", TileOptionActions::closeDoor);
	m_tileOptions.addOption(7, "Look At", std::bind(&MessageLog::addMessage, "A solid wooden door, currently open"));
	m_tileOptions.addOption(7, "Examine", std::bind(&MessageLog::addMessage, "Anyone could easily get through here"));
	m_tileOptions.addOption(7, "Durability", TileOptionActions::examineHealth);

	// Walls
	m_tileOptions.addOption(3, "Build window", TileOptionActions::build);
	m_tileOptions.addOption(3, "Examine", std::bind(&MessageLog::addMessage, "This wall is hard as bedrock"));
	m_tileOptions.addOption(3, "Durability", TileOptionActions::examineHealth);
	m_tileOptions.addOption(4, "Build window", TileOptionActions::build);
	m_tileOptions.addOption(4, "Examine", std::bind(&MessageLog::addMessage, "No way through this, not even after a few hits"));
	m_tileOptions.addOption(4, "Durability", TileOptionActions::examineHealth);

	// Window
	m_tileOptions.addOption(5, "Repair", TileOptionActions::repair);
	m_tileOptions.addOption(5, "Examine", std::bind(&MessageLog::addMessage, "A glass window, perfect for shooting out through"));
	m_tileOptions.addOption(5, "Durability", TileOptionActions::examineHealth);

	// Ammo box
	m_tileOptions.addOption(9, "Reload", TileOptionActions::reload);
	m_tileOptions.addOption(9, "Look at", std::bind(&MessageLog::addMessage, "A box full of batteries"));
	m_tileOptions.addOption(9, "Examine", std::bind(&MessageLog::addMessage, "The display on top keeps track of the battery quantity"));

	// Bed
	m_tileOptions.addOption(10, "Sleep", TileOptionActions::sleep);

	// Workbench
	m_tileOptions.addOption(11, "Craft", TileOptionActions::craft);
}

void GameState::reloadView()
{
	m_view.setSize(m_tileMap.getMapSize().x*m_tileMap.getTileSize(), m_tileMap.getMapSize().y*m_tileMap.getTileSize());
	m_view.setCenter((m_tileMap.getMapSize().x*m_tileMap.getTileSize())/2, (m_tileMap.getMapSize().y*m_tileMap.getTileSize())/2);
	pause();
	m_view.zoom(10);

	// Store ratio between map and window size
	sf::Vector2f viewSize(
		(double)m_view.getSize().x,
		(double)m_view.getSize().y);
	sf::Vector2f windowSize(
		(double)getEngine()->getWindow().getSize().x,
		(double)getEngine()->getWindow().getSize().y);
	jl::Settings::setDouble("gameRatio", 
		(viewSize.y/viewSize.x)/(windowSize.y/windowSize.x));
}

void GameState::events()
{
	if(!isPaused())
	{
		m_characters.events(getEngine()->getEvent());
		m_workbench.events(getEngine()->getEvent());
		SettingsConsole::events(getEngine()->getEvent(), *this);

		if(!m_characters.getPlayer().isDead())
			m_tileOptions.events(getEngine()->getEvent());
	}
}

void GameState::update()
{
	// Slowly fade in music
	getEngine()->getAssets().getMusic("res/Saturday Supernova.wav").setVolume(jl::Math::lerp(
		getEngine()->getAssets().getMusic("res/Saturday Supernova.wav").getVolume(), 45, getEngine()->getDelta()));

	// Smack down to planet (with camera)
	if(isPaused())
	{
		GalaxyGenerator::rotate(getEngine()->getDelta());

		m_view.setSize(jl::Vec::lerp(
			m_view.getSize(),
			sf::Vector2f(
				m_tileMap.getMapSize().x*m_tileMap.getTileSize(), 
				m_tileMap.getMapSize().y*m_tileMap.getTileSize()), 5*getEngine()->getDelta()));

		sf::Vector2f sizeDistance(
			sf::Vector2f(
				m_tileMap.getMapSize().x*m_tileMap.getTileSize(), 
				m_tileMap.getMapSize().y*m_tileMap.getTileSize()) - m_view.getSize());

		float distance = jl::Vec::length(sizeDistance);
		if(distance < 600)
			ParticleManager::clearParticles();

		if(distance < 1)
		{
			resume();
			m_view.setSize(sf::Vector2f(
				m_tileMap.getMapSize().x*m_tileMap.getTileSize(), 
				m_tileMap.getMapSize().y*m_tileMap.getTileSize()));
			ParticleManager::clearParticles();
		}
	}
	else if(m_characters.getPlayer().isDead())
	{
		if(!m_updatedHighscore)
		{
			
				
			getEngine()->getAssets().getMusic("res/Saturday Supernova.wav").setVolume(0);
			getEngine()->getAssets().getMusic("res/Saturday Supernova.wav").stop();
			getEngine()->getAssets().getMusic("res/Saturday Supernova.wav").play();
			jl::Settings::setInt("gameScore", m_characters.getPlayer().getScore());

			if(m_characters.getPlayer().getScore() > jl::Settings::getInt("gameHighscore"))
			{
				std::ofstream output("res/highscore.encrypted");
				output << "score=" << jl::Settings::getInt("gameScore");
				jl::Settings::setInt("gameHighscore", jl::Settings::getInt("gameScore"));
				output.close();
			}

			m_updatedHighscore = true;
			jl::SoundManager::playSound("res/death.wav");
		}

		// Fade in music to menu volume
		getEngine()->getAssets().getMusic("res/Saturday Supernova.wav").setVolume(jl::Math::lerp(
			getEngine()->getAssets().getMusic("res/Saturday Supernova.wav").getVolume(), 20, 2*getEngine()->getDelta()));

		GalaxyGenerator::rotate(getEngine()->getDelta()*5);
		m_backgroundPlanet.setPosition(
		m_view.getCenter().x - m_backgroundPlanet.getGlobalBounds().width/2,
		m_view.getCenter().y - m_backgroundPlanet.getGlobalBounds().height/2);

		m_view.setSize(jl::Vec::lerp(
			m_view.getSize(),
			sf::Vector2f(
				m_tileMap.getMapSize().x*m_tileMap.getTileSize()*10, 
				m_tileMap.getMapSize().y*m_tileMap.getTileSize()*10), 5*getEngine()->getDelta()));

		sf::Vector2f sizeDistance(
			sf::Vector2f(
				m_tileMap.getMapSize().x*m_tileMap.getTileSize()*10, 
				m_tileMap.getMapSize().y*m_tileMap.getTileSize()*10) - m_view.getSize());

		float distance = jl::Vec::length(sizeDistance);

		if(distance < 0.5)
		{
			m_view.setSize(
				m_tileMap.getMapSize().x*m_tileMap.getTileSize()*10, 
				m_tileMap.getMapSize().y*m_tileMap.getTileSize()*10);
			getEngine()->getStack().popState();
		}

		for(int i = 0; i < 5; i++)
			ParticleManager::addParticle(
				m_view.getCenter(),
				jl::Math::randDouble(1.0, 1.4),
				jl::Math::randInt(700, 900),
				0,
				jl::Math::randInt(0, 360),
				sf::Color(255, 102, 51),
				sf::Vector2f(m_backgroundPlanet.getScale().x, m_backgroundPlanet.getScale().y),
				1.4,
				true);
	}



	// Update view
	getEngine()->getWindow().setView(m_view);
	ParticleManager::update(getEngine()->getDelta());

	if(!isPaused() && !m_characters.getPlayer().isDead())
	{
		m_characters.update(getEngine()->getDelta());
		m_loot.update(getEngine()->getDelta());
		m_enemyWaves.update(getEngine()->getDelta());
		if(!m_characters.getPlayer().isDead())
		{
			m_workbench.update(getEngine()->getDelta());
			m_bedControl.update();
		}

		// Derp functions
		/*if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			m_view.zoom(1.0 - getEngine()->getDelta());
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
			m_view.zoom(1.0 + getEngine()->getDelta());

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			m_view.rotate(100*getEngine()->getDelta());

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::X))
			m_view.zoom(1.0);*/

		MessageLog::update(getEngine()->getDelta());
	}

}
void GameState::render()
{
	// Draw planet and galaxy during zoom in
	if(isPaused() || m_characters.getPlayer().isDead())
	{
		GalaxyGenerator::render(getEngine()->getWindow());

		ParticleManager::render(getEngine()->getWindow());

		getEngine()->getWindow().draw(m_backgroundPlanet);

	}

	if(!m_characters.getPlayer().isDead())
	{
		m_tileMap.render(getEngine()->getWindow());

		// Don't draw particles ontop tilemap during zoom in
		if(!isPaused())
			ParticleManager::render(getEngine()->getWindow());

		m_characters.render(getEngine()->getWindow());

		m_workbench.render(getEngine()->getWindow());
		m_tileOptions.render(getEngine()->getWindow());

		m_loot.render(getEngine()->getWindow());
		MessageLog::render(getEngine()->getWindow());

		m_enemyWaves.render(getEngine()->getWindow());
	}


	SettingsConsole::render(getEngine()->getWindow());
}

TileMap &GameState::getTileMap()
{
	return m_tileMap;
}
CharacterManager &GameState::getChars()
{
	return m_characters;
}
BedControl &GameState::getBed()
{
	return m_bedControl;
}
Workbench &GameState::getWorkbench()
{
	return m_workbench;
}
LootManager &GameState::getLoot()
{
	return m_loot;
}
TileOptionManager &GameState::getTileOptions()
{
	return m_tileOptions;
}
