#ifndef TILE_H
#define TILE_H

#include "TileCharacter.h"

class Tile
{
private:
	bool m_isOccupied, m_isSolid;
	int m_tileType;
	TileCharacter *m_character;

public:

	explicit Tile();

	void setSolid(bool solid);
	void setOccupied(bool occupied);

	void setTileType(int tiletype);

	void setCharacter(TileCharacter *character);
	void clearCharacter();
	bool hasCharacter() const;

	TileCharacter *getCharacter();
	int getTileType() const;
	bool isSolid() const;
	bool isOccupied() const;
};

#endif
