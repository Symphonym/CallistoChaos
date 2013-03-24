#ifndef TILEOPTIONACTIONS_H
#define TILEOPTIONACTIONS_H

#include <SFML/Graphics.hpp>

class TileMap;
namespace TileOptionActions
{
	void openDoor(TileMap *tileMap, const sf::Vector2i &tileIndex);
	void closeDoor(TileMap *tileMap, const sf::Vector2i &tileIndex);

	void examineHealth(TileMap *tileMap, const sf::Vector2i &tileIndex);
	void repair(TileMap *tileMap, const sf::Vector2i &tileIndex);
	void build(TileMap *tileMap, const sf::Vector2i &tileIndex);
};

#endif