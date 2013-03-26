#ifndef CHARACTERMANAGER_H
#define CHARACTERMANAGER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <memory>
#include "TileCharacter.h"

class TileMap;
class CharacterManager
{
private:

	// Vector of all the characters
	std::map<int, std::unique_ptr<TileCharacter>> m_characters;

	// Pointer to the Tilemap that the characters are in
	TileMap *m_tileMap;

	int m_charId;

	// Stops a certain character at a certain Tile
	void stopCharacter(TileCharacter &tilecharacter, const sf::Vector2f &tilePosition);

public:

	explicit CharacterManager();

	void registerTileMap(TileMap &tilemap);
	void addCharacter(std::unique_ptr<TileCharacter> tilecharacter);

	void events(sf::Event &events);
	void update(double deltaTime);
	void render(sf::RenderTarget &target);

	TileCharacter& getCharacter(int index);
};

#endif