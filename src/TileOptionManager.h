#ifndef TILEOPTIONMANAGER_H
#define TILEOPTIONMANAGER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <vector>
#include <functional>
#include "Player.h"
#include "AssetManager.h"

class Tile;
class TileOptionManager
{
private:

	typedef std::function<void(TileMap *tileMap, const sf::Vector2i &tileIndex)> ActionPtr;

	std::map<int, std::vector<std::pair<std::string, ActionPtr>>> m_tileOptions;
	static Player *m_player;

	// Whether or not to render the options
	bool m_displayOptions;

	// Selected item in the list of options
	int m_optionIndex;
	// Selected tile
	sf::Vector2i m_tileIndex;
	// Selected tile's tileType
	std::size_t m_tileType;

	// Data to render the list
	sf::Sprite m_sprite;
	sf::IntRect m_selRect, m_nonRect; // Guisheet subrects
	sf::Text m_text;

	// Gets the index of the tile that the character is facing
	sf::Vector2i getFacingTile();
	// Gets the type of the tile at the m_tileIndex pos
	int getTileType();
	// Sets all data to display the list
	void displayList();

public:

	explicit TileOptionManager();

	void loadAssets(jl::AssetManager &assets);

	// Map an option to a tiletype
	void addOption(int tileTypeIndex, const std::string &title, ActionPtr action);

	// The Player that will interact with the tiles
	static void provideCharacter(Player *player);

	// Update input
	void events(sf::Event &events);
	// Render the tile options
	void render(sf::RenderTarget &target);

	// Returns whether or not the tile options are visible
	bool isVisible();

	static Player *getPlayer();
};

#endif