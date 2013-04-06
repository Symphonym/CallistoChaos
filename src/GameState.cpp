#include "GameState.h"
#include "Player.h"
#include "MessageLog.h"
#include "TileOptionActions.h"
#include "WeakEnemy.h"
#include "Utility.h"

GameState::GameState(jl::Engine *engine) : 
	jl::State(engine),
	m_gameRatio(0.0),
	m_loot(engine->getAssets())
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
	{0,0,0,0,0,0,0,0,0,0,3,9,10,8,12,3,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,3,8,8,8,8,3,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,3,11,8,8,8,3,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
	// Throw in some random flowers at certain intervalls
	for(std::size_t y = 0; y < gameLevel.size(); y++)
		for(std::size_t x = 0; (std::size_t)x < gameLevel[x].size(); x++)
			if(!(x > 9 && x < 16 && y > 7 && y < 14)) // Set an area not have bushes generated
				gameLevel[y][x] = std::rand() % 100 < 20 ? (std::rand() % 100 < 40 ? 1 : 2) : 0;

	// Specify tilemap data
	m_tileMap.setData(
		getEngine()->getAssets().getAsset<jl::TextureAsset>("res/tiles.png")->get(),
		sf::Vector2i(24, 20),
		16);

	// Set camera data
	m_view.setSize(gameLevel[0].size()*m_tileMap.getTileSize(), gameLevel.size()*m_tileMap.getTileSize());
	m_view.setCenter((gameLevel[0].size()*m_tileMap.getTileSize())/2, (gameLevel.size()*m_tileMap.getTileSize())/2);

	// Store ratio between map and window size
	sf::Vector2f viewSize(
		(double)m_view.getSize().x,
		(double)m_view.getSize().y);
	sf::Vector2f windowSize(
		(double)getEngine()->getWindow().getSize().x,
		(double)getEngine()->getWindow().getSize().y);
	jl::Settings::setDouble("gameRatio", 
		(viewSize.y/viewSize.x)/(windowSize.y/windowSize.x));
	m_gameRatio = jl::Settings::getDouble("gameRatio");


	m_tileMap.addType(0, sf::IntRect(0,0, 16, 16)); // Ground
	m_tileMap.addType(1, sf::IntRect(16,0, 16, 16)); // Flower
	m_tileMap.addDType(2, sf::IntRect(32,0, 16, 16), sf::IntRect(16,48, 16, 16), 100, true, true); // Bush
	m_tileMap.addType(3, sf::IntRect(0, 16, 16, 16), true, true); // Wall side
	m_tileMap.addType(4, sf::IntRect(16, 16, 16, 16), true, true); // Wall top
	m_tileMap.addDType(5, sf::IntRect(32, 16, 16, 16), sf::IntRect(32,0, 16, 16), 5, false, true); // Window
	m_tileMap.addDType(6, sf::IntRect(0, 32, 16, 16), sf::IntRect(48, 32, 16, 16), 10, true, true); // Door closed
	m_tileMap.addType(7, sf::IntRect(16, 32, 16, 16)); // Door open
	m_tileMap.addType(8, sf::IntRect(32, 32, 16, 16)); // Floor
	m_tileMap.addType(9, sf::IntRect(32, 48, 16, 16), false, true); // Currency box
	m_tileMap.addType(10, sf::IntRect(48, 48, 16, 16), false, true); // Ammo box
	m_tileMap.addType(11, sf::IntRect(0, 48, 16, 16), false, true); // Bed
	m_tileMap.addType(12, sf::IntRect(0, 64, 16, 16), false, true); // Workbench
	m_tileMap.loadFromData(gameLevel);

	// Load assets
	m_tileOptions.loadAssets(getEngine()->getAssets());
	MessageLog::loadAssets(getEngine()->getAssets());

	m_scoreText.setFont(getEngine()->getAssets().getAsset<jl::FontAsset>("res/Minecraftia.ttf")->get());

	// Load character manager
	m_characters.registerTileMap(m_tileMap);
	std::unique_ptr<Player> player(new Player(this, getEngine()->getAssets(), sf::Vector2i(8,11)));

	// Set player to be used for the Tile options
	m_tileOptions.provideCharacter(player.get());
	m_loot.providePlayer(player.get());
	m_workbench = Workbench(player.get(), getEngine()->getAssets());
	m_bedControl = BedControl(player.get());

	// Give player to manager
	m_characters.addPlayer(std::move(player));


	//m_characters.addCharacter(std::unique_ptr<WeakEnemy>(new WeakEnemy(this, getEngine()->getAssets(), sf::Vector2i(0,0))));
	//m_characters.addCharacter(std::unique_ptr<WeakEnemy>(new WeakEnemy(this, getEngine()->getAssets(), sf::Vector2i(15,0))));
	//m_characters.addCharacter(std::unique_ptr<WeakEnemy>(new WeakEnemy(this, getEngine()->getAssets(), sf::Vector2i(0,8))));
	//m_characters.addCharacter(std::unique_ptr<WeakEnemy>(new WeakEnemy(this, getEngine()->getAssets(), sf::Vector2i(5,15))));
	//m_characters.addCharacter(std::unique_ptr<WeakEnemy>(new WeakEnemy(this, getEngine()->getAssets(), sf::Vector2i(15,15))));

	for(int i = 0; i < 1; i++)
		m_loot.spawnEntity(sf::Vector2f(50,50));

	

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

	// Currency box
	m_tileOptions.addOption(9, "Look at", std::bind(&MessageLog::addMessage, "A box full of metal scrap"));
	m_tileOptions.addOption(9, "Examine", std::bind(&MessageLog::addMessage, "The box displays the quantity of it's contents"));

	// Ammo box
	m_tileOptions.addOption(10, "Reload", TileOptionActions::reload);
	m_tileOptions.addOption(10, "Look at", std::bind(&MessageLog::addMessage, "A box full of batteries"));
	m_tileOptions.addOption(10, "Examine", std::bind(&MessageLog::addMessage, "The display on top keeps track of the battery quantity"));

	// Bed
	m_tileOptions.addOption(11, "Sleep", TileOptionActions::sleep);

	// Workbench
	m_tileOptions.addOption(12, "Craft", TileOptionActions::craft);
}

void GameState::events()
{
	if(!isPaused())
	{
		m_characters.events(getEngine()->getEvent());
		m_workbench.events(getEngine()->getEvent());
		m_tileOptions.events(getEngine()->getEvent());
	}
}

void GameState::update()
{
	// Update view
	getEngine()->getWindow().setView(m_view);

	if(!isPaused())
	{
		sf::Color scoreColor = sf::Color::White;
		scoreColor.a = m_characters.getPlayer().isDead() ? 255 : 150;
		m_scoreText.setColor(scoreColor);
		m_scoreText.setCharacterSize(m_characters.getPlayer().isDead() ? 60*m_gameRatio : 30*m_gameRatio);
		m_scoreText.setString("Score: " + jl::Util::toString(m_characters.getPlayer().getScore()));
		m_scoreText.setPosition(getEngine()->getWindow().getSize().x * 0.5 - int(m_scoreText.getGlobalBounds().width/2), getEngine()->getWindow().getSize().y * 0.1);

		m_characters.update(getEngine()->getDelta());
		m_workbench.update(getEngine()->getDelta());
		m_loot.update(getEngine()->getDelta());
		m_bedControl.update();

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			m_view.zoom(1.0 - getEngine()->getDelta());
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
			m_view.zoom(1.0 + getEngine()->getDelta());

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			m_view.rotate(100*getEngine()->getDelta());

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::X))
			m_view.zoom(1.0);

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::T))
			getEngine()->getStack().popState();

		MessageLog::update(getEngine()->getDelta());
	}

}
void GameState::render()
{
	m_tileMap.render(getEngine()->getWindow());
	m_characters.render(getEngine()->getWindow());
	m_workbench.render(getEngine()->getWindow());
	m_tileOptions.render(getEngine()->getWindow());
	m_loot.render(getEngine()->getWindow());
	MessageLog::render(getEngine()->getWindow());

	if(m_characters.getPlayer().isDead())
	{
		sf::Color blackColor = sf::Color::Black;
		blackColor.a = 100;
		sf::Image blackScreen;
		blackScreen.create(getEngine()->getWindow().getView().getSize().x, getEngine()->getWindow().getView().getSize().y, blackColor);

		sf::Texture blackScreenTexture; blackScreenTexture.loadFromImage(blackScreen);
		sf::Sprite blackScreenSprite; blackScreenSprite.setTexture(blackScreenTexture);
		getEngine()->getWindow().draw(blackScreenSprite);
	}

	getEngine()->getWindow().setView(getEngine()->getWindow().getDefaultView());
	getEngine()->getWindow().draw(m_scoreText);
	getEngine()->getWindow().setView(m_view);
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