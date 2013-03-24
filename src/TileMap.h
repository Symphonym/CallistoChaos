#ifndef TILEMAP_H
#define TILEMAP_H

#include <SFML/Graphics.hpp>
#include "Tile.h"

class TileMap
{
private:

	struct TileData
	{
		sf::IntRect subRect, destroyedSubRect;
		bool isSolid;
		bool isImmortal;
		int maxHealth;
	};

	// Tilesize and mapsize
	std::size_t m_tileSize;
	sf::Vector2i m_mapSize;
	// Sprite used when rendering tilemap
	sf::Sprite m_tileSprite;
	// Tiletypes stored with some individual data
	std::map<int, TileData> m_tileTypes;

	// Vector of tiles
	std::vector<std::vector<Tile>> m_tiles;

public:

	explicit TileMap();

	void setData(const sf::Texture &tilesheet, const sf::Vector2i &mapSize, std::size_t tileSize);

	// Add Tile type
	void addType(int tiletypeIndex, const sf::IntRect &subrect, bool isSolid = false);
	// Add Destructable Tile type
	void addDType(int tiletypeIndex, const sf::IntRect &subrect, const sf::IntRect &destroyedSubRect, int maxHealth, bool isSolid = false);

	void loadFromData(std::vector<std::vector<int>> &mapData);

	void changeTile(int tileType, std::size_t x, std::size_t y, bool resetHealth = false);
	void changeTile(int tileType, Tile *tile, bool resetHealth = false);

	Tile &getTile(std::size_t x, std::size_t y);
	Tile &getTile(const sf::Vector2i &tileIndex);

	sf::Vector2f getTilePosition(std::size_t x, std::size_t y) const;
	std::size_t getTileSize() const;
	sf::Vector2i getMapSize() const;

	void render(sf::RenderTarget& target);
};

#endif