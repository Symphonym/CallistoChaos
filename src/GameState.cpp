#include "GameState.h"
#include "Player.h"
#include "MessageLog.h"

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
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
	// Throw in some random flowers at certain intervalls
	for(int x = 0; (std::size_t)x < gameLevel.size(); x++)
		for(int y = 0; (std::size_t)y < gameLevel[x].size(); y++)
			if(!(x >= 4 && x <= 12 && y >= 5 && y <= 12)) // Set an area not have bushes generated
				gameLevel[x][y] = std::rand() % 100 < 10 ? (std::rand() % 100 < 40 ? 1 : 2) : 0;

	// Set camera zoom
	m_view = getEngine()->createView(0.32);

	// Specify tilemap data
	m_tileMap.setData(
		getEngine()->getAssets().getAsset<jl::TextureAsset>("res/tiles.png")->get(),
		sf::Vector2i(20, 20),
		16);
	m_tileMap.addTileType(0, sf::IntRect(0,0, 16, 16)); // Ground
	m_tileMap.addTileType(1, sf::IntRect(16,0, 16, 16)); // Flower
	m_tileMap.addTileType(2, sf::IntRect(32,0, 16, 16), true); // Bush
	m_tileMap.loadFromData(gameLevel);

	// Load assets
	m_tileOptions.loadAssets(getEngine()->getAssets());
	MessageLog::loadAssets(getEngine()->getAssets());

	// Load character manager
	m_characters.registerTileMap(m_tileMap);
	std::unique_ptr<Player> player(new Player(m_tileMap, getEngine()->getAssets(), sf::Vector2i(0,0)));
	m_characters.addCharacter(std::move(player));

	// Set character to be used for the Tile options
	m_tileOptions.provideCharacter(&m_characters.getCharacter(0));


	m_tileOptions.addOption(2, "Look at", std::bind(&MessageLog::addMessage, "Looks like a bush"));
	m_tileOptions.addOption(2, "Examine", std::bind(&MessageLog::addMessage, "It's still a bush"));

	m_tileOptions.addOption(1, "Look at", std::bind(&MessageLog::addMessage, "A small flower"));
	m_tileOptions.addOption(1, "Examine", std::bind(&MessageLog::addMessage, "Still looks like a flower"));
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