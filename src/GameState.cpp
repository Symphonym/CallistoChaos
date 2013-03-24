#include "GameState.h"
#include "Player.h"
#include "MessageLog.h"
#include "TileOptionActions.h"

GameState::GameState(jl::Engine *engine) : jl::State(engine)
{

	std::vector<std::vector<int>> gameLevel = {
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,3,4,4,6,4,3,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,3,9,10,8,8,3,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,3,8,8,8,8,3,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,3,8,8,8,8,3,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,4,4,4,4,4,4,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
	// Throw in some random flowers at certain intervalls
	for(int y = 0; (std::size_t)y < gameLevel.size(); y++)
		for(int x = 0; (std::size_t)x < gameLevel[x].size(); x++)
			if(!(x > 6 && x < 13 && y > 7 && y < 14)) // Set an area not have bushes generated
				gameLevel[y][x] = std::rand() % 100 < 15 ? (std::rand() % 100 < 40 ? 1 : 2) : 0;

	// Set camera zoom
	m_view = getEngine()->createView(0.32);

	// Specify tilemap data
	m_tileMap.setData(
		getEngine()->getAssets().getAsset<jl::TextureAsset>("res/tiles.png")->get(),
		sf::Vector2i(20, 20),
		16);
	m_tileMap.addType(0, sf::IntRect(0,0, 16, 16)); // Ground
	m_tileMap.addType(1, sf::IntRect(16,0, 16, 16)); // Flower
	m_tileMap.addDType(2, sf::IntRect(32,0, 16, 16), sf::IntRect(32,0, 16, 16), 100, true); // Bush
	m_tileMap.addType(3, sf::IntRect(0, 16, 16, 16), true); // Wall side
	m_tileMap.addType(4, sf::IntRect(16, 16, 16, 16), true); // Wall top
	m_tileMap.addDType(5, sf::IntRect(32, 16, 16, 16), sf::IntRect(32,0, 16, 16), 5, true); // Window
	m_tileMap.addDType(6, sf::IntRect(0, 32, 16, 16), sf::IntRect(48, 32, 16, 16), 10, true); // Door closed
	m_tileMap.addType(7, sf::IntRect(16, 32, 16, 16)); // Door open
	m_tileMap.addType(8, sf::IntRect(32, 32, 16, 16)); // Floor
	m_tileMap.addType(9, sf::IntRect(32, 48, 16, 16), true); // Currency box
	m_tileMap.addType(10, sf::IntRect(48, 48, 16, 16), true); // Ammo box
	m_tileMap.loadFromData(gameLevel);

	// Load assets
	m_tileOptions.loadAssets(getEngine()->getAssets());
	MessageLog::loadAssets(getEngine()->getAssets());

	// Load character manager
	m_characters.registerTileMap(m_tileMap);
	std::unique_ptr<Player> player(new Player(m_tileMap, getEngine()->getAssets(), sf::Vector2i(0,0)));
	// Set player to be used for the Tile options
	TileOptionManager::provideCharacter(player.get());
	m_characters.addCharacter(std::move(player));



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
	m_tileOptions.addOption(6, "Damage", TileOptionActions::damageDoor);
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

	// Currency box
	m_tileOptions.addOption(10, "Look at", std::bind(&MessageLog::addMessage, "A box full of batteries"));
	m_tileOptions.addOption(10, "Examine", std::bind(&MessageLog::addMessage, "The display on top keeps track of the battery quantity"));
}

void GameState::events()
{
	m_characters.events(getEngine()->getEvent());
	m_tileOptions.events(getEngine()->getEvent());
}
void GameState::update()
{
	m_characters.update(getEngine()->getDelta());

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		m_view.zoom(1.0 - getEngine()->getDelta());
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		m_view.zoom(1.0 + getEngine()->getDelta());

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		m_view.rotate(100*getEngine()->getDelta());

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::T))
		getEngine()->getStack().popState();

	// Update view
	getEngine()->getWindow().setView(m_view);

	MessageLog::update(getEngine()->getDelta());
}
void GameState::render()
{
	m_tileMap.render(getEngine()->getWindow());
	m_characters.render(getEngine()->getWindow());
	m_tileOptions.render(getEngine()->getWindow());
	MessageLog::render(getEngine()->getWindow());
}