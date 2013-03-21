#include "Tilemap.h"

namespace jl
{
	Tilemap::Tilemap() :
		m_tileSize(0,0),
		m_mapSize(0,0),
		m_startIndex(0,0),
		m_endIndex(0,0),
		m_tileSprite(),
		m_tileTypes(),
		m_tileSheet(nullptr),
		m_tiles;
	{

	}
	Tilemap(const sf::Vector2i &map, const sf::Vector2i &map);

	void setTileSize(std::size_t size);
	void setMapSize(const sf::Vector2i &map, const sf::Vector2i &map);

	void cullTiles(const sf::Vector2i &cameraPosition, const sf::Vector2i &cameraSize);
	void resetCulling() const;

	void setTileSheet(sf::Texture &tilesheet);
	void addTileType(int tiletypeIndex, const sf::IntRect &subrect);

	void loadFromFile(const std::string &filepath);
	void loadFromData(const std::vector<const std::vector<jl::Tile>> &mapData);

	virtual void draw(sf::RenderTarget& target, RenderStates states) const
	{
		for(int x = m_startIndex.x; x < m_endIndex.x; x++)
		{
			for(int y = m_startIndex.y; y < m_endIndex.y; y++)
			{
				// Set position, set subrect on tilesheet and render
				m_tileSprite.setPosition(x * m_tilesize.x, y * m_tilesize.y);
				if(m_tiletypes)
				m_tileSprite.setTextureRect(m_tiletypes[m_tiles[x][y].tiletype]);
				target.draw(m_tileSprite, states);
			}
		}
	}
};	