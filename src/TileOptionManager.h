#ifndef TILEOPTIONMANAGER_H
#define TILEOPTIONMANAGER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <vector>
#include <functional>
#include "TileCharacter.h"
#include "AssetManager.h"

class TileOptionManager
{
private:

	std::map<int, std::vector<std::pair<std::string, std::function<void()>>>> m_tileOptions;
	TileCharacter *m_tileCharacter;

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

public:

	explicit TileOptionManager();

	void loadAssets(jl::AssetManager &assets);

	// Map an option to a tiletype
	void addOption(int tileTypeIndex, const std::string &title, std::function<void()> action);

	// The character that will interact with the tiles
	void provideCharacter(TileCharacter *tilecharacter);

	// Update input
	void events(sf::Event &events);
	// Render the tile options
	void render(sf::RenderTarget &target);

	// Returns whether or not the tile options are visible
	bool isVisible();
};

#endif