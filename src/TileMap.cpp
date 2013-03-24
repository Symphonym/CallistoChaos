#include "TileMap.h"
#include <stdexcept>

TileMap::TileMap() :
	m_tileSize(0),
	m_mapSize(0,0)
{
}

void TileMap::setData(const sf::Texture &tilesheet, const sf::Vector2i &mapSize, std::size_t tileSize)
{
	m_tileSprite.setTexture(tilesheet);
	m_tileSize = tileSize;
	m_mapSize = mapSize;

	// Fill tilemap with default tiles
	Tile tile;
	m_tiles.resize(m_mapSize.y, std::vector<Tile>(m_mapSize.x, tile));
}
void TileMap::addType(int tiletypeIndex, const sf::IntRect &subrect, bool isSolid)
{
	TileData data;
	data.subRect = subrect;
	data.destroyedSubRect = sf::IntRect(0,0,0,0);
	data.isSolid = isSolid;
	data.isImmortal = true;
	data.maxHealth = 0;

	m_tileTypes[tiletypeIndex] = data;
}
void TileMap::addDType(int tiletypeIndex, const sf::IntRect &subrect, const sf::IntRect &destroyedSubRect, int maxHealth, bool isSolid)
{
	TileData data;
	data.subRect = subrect;
	data.destroyedSubRect = destroyedSubRect;
	data.isSolid = isSolid;
	data.isImmortal = false;
	data.maxHealth = maxHealth;

	m_tileTypes[tiletypeIndex] = data;
}

void TileMap::loadFromData(std::vector<std::vector<int>> &mapData)
{
	if(mapData.size() != (std::size_t)m_mapSize.x || mapData[0].size() != (std::size_t)m_mapSize.y ||
	   mapData.size() > (std::size_t)m_mapSize.x || mapData[0].size() > (std::size_t)m_mapSize.y)
		throw std::runtime_error("TileMap data dit not fit within TileMap.");

	for(int y = 0; y < m_mapSize.y; y++)
	{
		for(int x = 0; x < m_mapSize.x; x++)
		{
			Tile *tile(&m_tiles[y][x]);
			tile->setTileType(mapData[y][x]);

			// Set Tiletype specific data
			if(!m_tileTypes.empty())
			{
				tile->setSolid(m_tileTypes[tile->getTileType()].isSolid);
				tile->setImmortal(m_tileTypes[tile->getTileType()].isImmortal);
				tile->setMaxHealth(m_tileTypes[tile->getTileType()].maxHealth);
			}
		}
	}
}

void TileMap::changeTile(int tileType, std::size_t x, std::size_t y, bool resetHealth)
{
	getTile(x, y).setTileType(tileType);

	if(resetHealth)
		getTile(x, y).setMaxHealth(m_tileTypes[tileType].maxHealth);

	getTile(x, y).setImmortal(m_tileTypes[tileType].isImmortal);
	getTile(x, y).setSolid(m_tileTypes[tileType].isSolid);
}
void TileMap::changeTile(int tileType, Tile *tile, bool resetHealth)
{
	tile->setTileType(tileType);

	if(resetHealth)
		tile->setMaxHealth(m_tileTypes[tileType].maxHealth);

	tile->setImmortal(m_tileTypes[tileType].isImmortal);
	tile->setSolid(m_tileTypes[tileType].isSolid);
}

void TileMap::render(sf::RenderTarget& target)
{
	for(int y = 0; y < m_mapSize.y; y++)
	{
		for(int x = 0; x < m_mapSize.x; x++)
		{
			// Scale sprites
			m_tileSprite.setScale(
				(float)m_tileSize / m_tileTypes[m_tiles[y][x].getTileType()].subRect.width,
				(float)m_tileSize / m_tileTypes[m_tiles[y][x].getTileType()].subRect.height);

			// Set position, set subrect on tilesheet and render tiles
			m_tileSprite.setPosition(x * m_tileSize, y * m_tileSize);
			if(!m_tileTypes.empty())
			{
				if(m_tiles[y][x].isImmortal())
					m_tileSprite.setTextureRect(m_tileTypes[m_tiles[y][x].getTileType()].subRect);
				else
				{
					m_tileSprite.setTextureRect(m_tiles[y][x].isDestroyed() ?
						m_tileTypes[m_tiles[y][x].getTileType()].destroyedSubRect : 
						m_tileTypes[m_tiles[y][x].getTileType()].subRect);

					if(m_tiles[y][x].isDestroyed())
						m_tiles[y][x].setSolid(false);
					else
						m_tiles[y][x].setSolid(m_tileTypes[m_tiles[y][x].getTileType()].isSolid);
				}
			}
			target.draw(m_tileSprite);
		}
	}
}

Tile &TileMap::getTile(std::size_t x, std::size_t y)
{
	return m_tiles[y][x];
}
Tile &TileMap::getTile(const sf::Vector2i &tileIndex)
{
	return m_tiles[tileIndex.y][tileIndex.x];
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