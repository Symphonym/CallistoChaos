#include "Tile.h"
#include "Tile.h"
#include "ParticleManager.h"
#include "Utility.h"
#include "TileMap.h"

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

void Tile::damage(int damage, TileMap *tileMap, const sf::Vector2i &index, int baseDirection)
{
	int directionAngle = baseDirection;
	int hitParticles = jl::Math::randInt(2, 4);
	int particleSpreadAngle = 40;
	double hpRatio = (double)getHealth()/(double)getMaxHealth();

	sf::Vector2f position(
		tileMap->getTilePosition(index.x, index.y).x + tileMap->getTileSize()/2,
		tileMap->getTilePosition(index.x, index.y).y + tileMap->getTileSize()/2);

	// Adjust color depending on enemy hp, the brighter the more damage
	sf::Color particleColor = sf::Color::White;

	// Red blood color
	if(isOccupied())
	{
		hpRatio = (double)getCharacter()->getHealth()/(double)getCharacter()->getMaxHealth();
		particleColor = sf::Color(255 - (125*hpRatio), 0 ,0);
		position = sf::Vector2f(
			getCharacter()->getSprite().getPosition().x + getCharacter()->getSprite().getGlobalBounds().width/2,
			getCharacter()->getSprite().getPosition().y + getCharacter()->getSprite().getGlobalBounds().height/2);
	}

	// Standard brown color
	else if(isImmortal())
		particleColor = sf::Color(138, 91, 0);

	// Brown tree/door/wall color
	else if(isPlayerAttackable())
		particleColor = sf::Color(138 - (57*hpRatio), 91 - (45*hpRatio), 0 - (-16*hpRatio));

	// Grey object color
	else
		particleColor = sf::Color(160 - (70*hpRatio), 160 - (70*hpRatio), 160 - (70*hpRatio));

	// No specified direction, add some extra particles and have 360 spread
	if(baseDirection == -1)
	{
		hitParticles = jl::Math::randInt(4, 7);
		particleSpreadAngle = 0;
	}

	for(int i = 0; i < hitParticles; i++)
	{
		double particleSpread = -particleSpreadAngle+((double)std::rand() / (double)RAND_MAX)*particleSpreadAngle*2;

		if(baseDirection == -1)
			directionAngle = std::rand() % 360;

		ParticleManager::addParticle(
			position,
			jl::Math::randInt(3, 8),
			jl::Math::randInt(30, 60),
			0,
			directionAngle + particleSpread,
			particleColor,
			sf::Vector2f(3, 3),
			0.5,
			true);

	}

	// Damage character on tile 
	if(isOccupied())
		m_character->damage(damage);
	
	if(m_isImmortal)
		return;

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