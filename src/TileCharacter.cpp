#include "TileCharacter.h"
#include "TileMap.h"

TileCharacter::TileCharacter(TileMap &tilemap) :
	m_tileMap(&tilemap),
	m_animation(),
	m_tileIndex(0,0),
	m_sprite(),
	m_isWalking(false),
	m_maxHealth(0),
	m_health(0),
	m_direction(TileCharacter::Down)
{

}

void TileCharacter::setMaxHealth(int health)
{
	m_maxHealth = health;
	m_health = m_maxHealth;
}
void TileCharacter::setSpeed(double speed)
{
	m_speed = speed;
}
void TileCharacter::setWalking(bool walking)
{
	m_isWalking = walking;
}
void TileCharacter::setDirection(TileCharacter::Directions direction)
{
	m_direction = direction;
}
void TileCharacter::setIndex(std::size_t x, std::size_t y)
{
	m_tileIndex = sf::Vector2i(x, y);
}
void TileCharacter::damage(int damage)
{
	m_health -= damage;
}

void TileCharacter::walkRight()
{
	int x = m_tileIndex.x + 1, y = m_tileIndex.y;
	if(x < m_tileMap->getMapSize().x)
	{
		if(!m_tileMap->getTile(x, y).isOccupied() &&
		   !m_tileMap->getTile(x, y).isSolid())
		{
			// Deoccupy previous tile
			m_tileMap->getTile(m_tileIndex.x, y).setOccupied(false);
			m_tileMap->getTile(m_tileIndex.x, y).clearCharacter();
			// Occupy new tile
			++m_tileIndex.x;
			m_tileMap->getTile(m_tileIndex.x, y).setOccupied(true);
			m_tileMap->getTile(m_tileIndex.x, y).setCharacter(this);
			
			// Set walking data
			setWalking(true);
			m_direction = TileCharacter::Right;

			doWalkRight();
		}
	}
}
void TileCharacter::walkLeft()
{
	int x = m_tileIndex.x - 1, y = m_tileIndex.y;
	if(x >= 0)
	{
		if(!m_tileMap->getTile(x, y).isOccupied() &&
		   !m_tileMap->getTile(x, y).isSolid())
		{
			// Deoccupy previous tile
			m_tileMap->getTile(m_tileIndex.x, y).setOccupied(false);
			m_tileMap->getTile(m_tileIndex.x, y).clearCharacter();
			// Occupy new tile
			--m_tileIndex.x;
			m_tileMap->getTile(m_tileIndex.x, y).setOccupied(true);
			m_tileMap->getTile(m_tileIndex.x, y).setCharacter(this);
						
			// Set walking data
			setWalking(true);
			m_direction = TileCharacter::Left;

			doWalkLeft();
		}
	}
}
void TileCharacter::walkUp()
{
	int x = m_tileIndex.x, y = m_tileIndex.y - 1;
	if(y >= 0)
	{
		if(!m_tileMap->getTile(x, y).isOccupied() &&
		   !m_tileMap->getTile(x, y).isSolid())
		{
			// Deoccupy previous tile
			m_tileMap->getTile(x, m_tileIndex.y).setOccupied(false);
			m_tileMap->getTile(x, m_tileIndex.y).clearCharacter();
			// Occupy new tile
			--m_tileIndex.y;
			m_tileMap->getTile(x, m_tileIndex.y).setOccupied(true);
			m_tileMap->getTile(x, m_tileIndex.y).setCharacter(this);
						
			// Set walking data
			setWalking(true);
			m_direction = TileCharacter::Up;

			doWalkUp();
		}
	}
}
void TileCharacter::walkDown()
{
	int x = m_tileIndex.x, y = m_tileIndex.y + 1;
	if(y < m_tileMap->getMapSize().y)
	{
		if(!m_tileMap->getTile(x, y).isOccupied() &&
		   !m_tileMap->getTile(x, y).isSolid())
		{
			// Deoccupy previous tile
			m_tileMap->getTile(x, m_tileIndex.y).setOccupied(false);
			m_tileMap->getTile(x, m_tileIndex.y).clearCharacter();
			// Occupy new tile
			++m_tileIndex.y;
			m_tileMap->getTile(x, m_tileIndex.y).setOccupied(true);
			m_tileMap->getTile(x, m_tileIndex.y).setCharacter(this);
						
			// Set walking data
			setWalking(true);
			m_direction = TileCharacter::Down;

			doWalkDown();
		}
	}
}

sf::Sprite &TileCharacter::getSprite()
{
	return m_sprite;
}
jl::FrameAnimation &TileCharacter::getAnim()
{
	return m_animation;
}
sf::Vector2i TileCharacter::getIndex() const
{
	return m_tileIndex;
}
TileCharacter::Directions TileCharacter::getDirection() const
{
	return m_direction;
}
double TileCharacter::getSpeed() const
{
	return m_speed;
}
bool TileCharacter::isWalking() const
{
	return m_isWalking;
}
bool TileCharacter::isDead() const
{
	return m_health <= 0;
}