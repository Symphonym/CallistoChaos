c#include "Tile.h"

Tile::Tile() :
	m_isSolid(false),
	m_isImmortal(false),
	m_isPlayerAttackable(false),
	m_tileType(0),
	m_health(0),
	m_maxHealth(0),
	m_character(nullptr)
{

}

void Tile::setSolid(bool solid)
{
	m_isSolid = solid;
}
void Tile::setImmortal(bool immortal)
{
	m_isImmortal = immortal;
}
void Tile::setPlayerAttackable(bool attackable)
{
	m_isPlayerAttackable = attackable;
}

void Tile::setMaxHealth(int maxHealth)
{
	m_maxHealth = maxHealth;
	m_health = m_maxHealth;
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

void Tile::damage(int damage)
{
	if(m_isImmortal)
		return;

	// Damage character on tile 
	if(isOccupied())
		m_character->damage(damage);
	
	m_health -= damage;

	if(m_health < 0)
		m_health = 0;
}
void Tile::repair(int repair)
{
	if(m_isImmortal)
		return;

	m_health += repair;

	if(m_health > m_maxHealth)
		m_health = m_maxHealth;
}

TileCharacter *Tile::getCharacter()
{
	return m_character;
}
int Tile::getTileType() const
{
	return m_tileType;
}
int Tile::getHealth() const
{
	return m_health;
}
int Tile::getMaxHealth() const
{
	return m_maxHealth;
}
bool Tile::isSolid() const
{
	return m_isSolid;
}
bool Tile::isOccupied() const
{
	return m_character != nullptr;
}
bool Tile::isImmortal() const
{
	return m_isImmortal;
}
bool Tile::isPlayerAttackable() const
{
	return m_isPlayerAttackable;
}
bool Tile::isDestroyed() const
{
	return m_health <= 0;
}