#ifndef TILEMAP_H
#define TILEMAP_H

#include <SFML/Graphics.hpp>
#include "Tile.h"

class TileMap
{
private:
	// Tilesize and mapsize
	std::size_t m_tileSize;
	sf::Vector2i m_mapSize;
	// Sprite used when rendering tilemap
	sf::Sprite m_tileSprite;
	// Tiletypes stored as an integer, mapped with an sf::IntRect and isSolid variable
	std::map<int, std::pair<sf::IntRect, bool>> m_tileTypes;

	// Vector of tiles
	std::vector<std::vector<Tile>> m_tiles;

public:

	explicit TileMap();

	void setData(sf::Texture &tilesheet, const sf::Vector2i &mapSize, std::size_t tileSize);

	void addTileType(int tiletypeIndex, const sf::IntRect &subrect, bool isSolid = false);

	void loadFromData(std::vector<std::vector<int>> &mapData);

	Tile &getTile(std::size_t x, std::size_t y);
	sf::Vector2f getTilePosition(std::size_t x, std::size_t y) const;
	std::size_t getTileSize() const;
	sf::Vector2i getMapSize() const;

	void render(sf::RenderTarget& target);
};

#endif