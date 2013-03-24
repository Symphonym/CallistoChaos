#ifndef TILE_H
#define TILE_H

#include "TileCharacter.h"

class Tile
{
private:
	bool m_isSolid, m_isImmortal;
	int m_tileType;
	int m_health, m_maxHealth;
	TileCharacter *m_character;

public:

	explicit Tile();

	void setSolid(bool solid);
	void setImmortal(bool immortal);
	
	void setMaxHealth(int maxHealth);
	void setTileType(int tiletype);

	void setCharacter(TileCharacter *character);
	void clearCharacter();

	void damage(int damage);
	void repair(int repair);

	TileCharacter *getCharacter();
	int getTileType() const;
	int getHealth() const;
	int getMaxHealth() const;
	bool isSolid() const;
	bool isOccupied() const;
	bool isImmortal() const;
	bool isDestroyed() const;
};

#endif
