#ifndef TILE_H
#define TILE_H

#include <SFML/Graphics.hpp>
#include "TileCharacter.h"

class TileMap;
class Tile
{
private:
	bool m_isSolid, m_isImmortal, m_isPlayerAttackable;
	int m_tileType;
	int m_health, m_maxHealth;
	TileCharacter *m_character;

public:

	explicit Tile();

	void setSolid(bool solid);
	void setImmortal(bool immortal);
	void setPlayerAttackable(bool attackable);
	
	void setMaxHealth(int maxHealth);
	void setTileType(int tiletype);

	void setCharacter(TileCharacter *character);
	void clearCharacter();

	// Base direction is used for splatter particle effects
	void damage(int damage, TileMap *tileMap, const sf::Vector2i &index, int baseDirection = -1);
	void repair(int repair);

	TileCharacter *getCharacter();
	int getTileType() const;
	int getHealth() const;
	int getMaxHealth() const;
	bool isSolid() const;
	bool isOccupied() const;
	bool isImmortal() const;
	bool isPlayerAttackable() const;
	bool isDestroyed() const;
};

#endif
