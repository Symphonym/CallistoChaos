#ifndef TILEOPTIONACTIONS_H
#define TILEOPTIONACTIONS_H

#include <SFML/Graphics.hpp>

class TileOptionManager;
class TileMap;
namespace TileOptionActions
{
	void openDoor(TileMap *tileMap, const sf::Vector2i &tileIndex, TileOptionManager *tileOptionManager);
	void closeDoor(TileMap *tileMap, const sf::Vector2i &tileIndex, TileOptionManager *tileOptionManager);

	void examineHealth(TileMap *tileMap, const sf::Vector2i &tileIndex, TileOptionManager *tileOptionManager);
	void repair(TileMap *tileMap, const sf::Vector2i &tileIndex, TileOptionManager *tileOptionManager);
	void build(TileMap *tileMap, const sf::Vector2i &tileIndex, TileOptionManager *tileOptionManager);

	void reload(TileMap *tileMap, const sf::Vector2i &tileIndex, TileOptionManager *tileOptionManager);
	void sleep(TileMap *tileMap, const sf::Vector2i &tileIndex, TileOptionManager *tileOptionManager);
	void craft(TileMap *tileMap, const sf::Vector2i &tileIndex, TileOptionManager *tileOptionManager);
};

#endif