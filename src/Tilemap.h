#ifndef JL_TILEMAP_H
#define JL_TILEMAP_H

#include <SFML/Graphics.hpp>

namespace jl
{	

	class Tilemap : sf::Drawable
	{
	private:
		// Tilesize and mapsize
		sf::Vector2i m_tileSize, m_mapSize;
		// Start and end index of rendering
		sf::Vector2i m_startIndex, m_endIndex;
		// Sprite used when rendering tilemap
		sf::Sprite m_tileSprite;
		// Tiletypes stored as an integer, mapped with an sf::IntRect
		std::map<int, sf::IntRect> m_tileTypes;
		// Pointer to tilesheet
		sf::Texture *m_tileSheet;

		// Vector of tiles
		std::vector<std::vector<jl::Tile>> m_tiles;

	public:

		explicit Tilemap();
		explicit Tilemap(const sf::Vector2i &map, const sf::Vector2i &map);

		void setTileSize(std::size_t size);
		void setMapSize(const sf::Vector2i &map, const sf::Vector2i &map);

		void cullTiles(const sf::Vector2i &cameraPosition, const sf::Vector2i &cameraSize);
		void resetCulling() const;

		void setTileSheet(sf::Texture &tilesheet);
		void addTileType(int tiletypeIndex, const sf::IntRect &subrect);

		void loadFromFile(const std::string &filepath);
		void loadFromData(const std::vector<const std::vector<jl::Tile>> &mapData);

		virtual void draw(sf::RenderTarget& target, RenderStates states) const;
	};
};

#endif