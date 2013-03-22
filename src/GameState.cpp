#include "GameState.h"
#include "SoundManager.h"
#include "Player.h"

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
	view = getEngine()->createView(0.32);

	// Specify tilemap data
	m_tileMap.setData(
		getEngine()->getAssets().getAsset<jl::TextureAsset>("Tiles.png")->get(),
		sf::Vector2i(20, 20),
		16);
	m_tileMap.addTileType(0, sf::IntRect(0,0, 16, 16)); // Ground
	m_tileMap.addTileType(1, sf::IntRect(16,0, 16, 16)); // Flower
	m_tileMap.addTileType(2, sf::IntRect(32,0, 16, 16), true); // Bush
	m_tileMap.loadFromData(gameLevel);



	// Register character manager
	m_characters.registerTileMap(m_tileMap);
	std::unique_ptr<Player> player(new Player(m_tileMap));
	player->getSprite().setTexture(getEngine()->getAssets().getAsset<jl::TextureAsset>("rpgmaker.png")->get());
	player->setIndex(0,0);
	m_characters.addCharacter(std::move(player));
}

void GameState::events()
{

}
void GameState::update()
{
	m_characters.update(getEngine()->getDelta());

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		view.zoom(1.0 - getEngine()->getDelta());
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		view.zoom(1.0 + getEngine()->getDelta());

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		view.rotate(100*getEngine()->getDelta());

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::T))
		getEngine()->getStack().popState();

	// Update view
	getEngine()->getWindow().setView(view);
}
void GameState::render()
{
	m_tileMap.render(getEngine()->getWindow());
	m_characters.render(getEngine()->getWindow());
}