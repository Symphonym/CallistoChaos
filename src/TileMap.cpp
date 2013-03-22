#include "TileMap.h"
#include <stdexcept>

TileMap::TileMap() :
	m_tileSize(0),
	m_mapSize(0,0)
{
}

void TileMap::setData(sf::Texture &tilesheet, const sf::Vector2i &mapSize, std::size_t tileSize)
{
	m_tileSprite.setTexture(tilesheet);
	m_tileSize = tileSize;
	m_mapSize = mapSize;

	// Fill tilemap with default tiles
	Tile tile;
	m_tiles.resize(m_mapSize.x, std::vector<Tile>(m_mapSize.y, tile));
}
void TileMap::addTileType(int tiletypeIndex, const sf::IntRect &subrect, bool solid)
{
	m_tileTypes[tiletypeIndex] = std::make_pair(subrect, solid);
}

void TileMap::loadFromData(std::vector<std::vector<int>> &mapData)
{
	if(mapData.size() != (std::size_t)m_mapSize.x || mapData[0].size() != (std::size_t)m_mapSize.y ||
	   mapData.size() > (std::size_t)m_mapSize.x || mapData[0].size() > (std::size_t)m_mapSize.y)
		throw std::runtime_error("TileMap data dit not fit within TileMap.");

	for(int x = 0; x < m_mapSize.x; x++)
	{
		for(int y = 0; y < m_mapSize.y; y++)
		{
			Tile tile;
			tile.setTileType(mapData[x][y]);

			// Check if tileType is solid
			if(!m_tileTypes.empty())
				tile.setSolid(m_tileTypes[tile.getTileType()].second);

			m_tiles[x][y] = tile;
		}
	}
}

Tile &TileMap::getTile(std::size_t x, std::size_t y)
{
	return m_tiles[x][y];
}
sf::Vector2f TileMap::getTilePosition(std::size_t x, std::size_t y) const
{
	return  sf::Vector2f(x * m_tileSize, y * m_tileSize);
}
std::size_t TileMap::getTileSize() const
{
	return m_tileSize;
}
sf::Vector2i TileMap::getMapSize() const
{
	return m_mapSize;
}

void TileMap::render(sf::RenderTarget& target)
{
	for(int x = 0; x < m_mapSize.x; x++)
	{
		for(int y = 0; y < m_mapSize.y; y++)
		{
			// Scale sprites
			m_tileSprite.setScale(
				(float)m_tileSize / m_tileTypes[m_tiles[x][y].getTileType()].first.width,
				(float)m_tileSize / m_tileTypes[m_tiles[x][y].getTileType()].first.height);

			// Set position, set subrect on tilesheet and render tiles
			m_tileSprite.setPosition(x * m_tileSize, y * m_tileSize);
			if(!m_tileTypes.empty())
				m_tileSprite.setTextureRect(m_tileTypes[m_tiles[x][y].getTileType()].first);
			target.draw(m_tileSprite);
		}
	}
}