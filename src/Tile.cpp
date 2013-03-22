#include "Tile.h"

Tile::Tile() :
	m_isOccupied(false),
	m_isSolid(false),
	m_tileType(0),
	m_character(nullptr)
{

}

void Tile::setSolid(bool solid)
{
	m_isSolid = solid;
}
void Tile::setOccupied(bool occupied)
{
	m_isOccupied = occupied;
}

void Tile::setTileType(int tiletype)
{
	m_tileType = tiletype;
}

void Tile::setCharacter(TileCharacter *character)
{
	m_character = character;
}
void Tile::clearCharacter()
{
	m_character = nullptr;
}
bool Tile::hasCharacter() const
{
	return m_character == nullptr;
}

TileCharacter *Tile::getCharacter()
{
	return m_character;
}
int Tile::getTileType() const
{
	return m_tileType;
}
bool Tile::isSolid() const
{
	return m_isSolid;
}
bool Tile::isOccupied() const
{
	return m_isOccupied;
}