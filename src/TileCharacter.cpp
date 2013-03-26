#include "TileCharacter.h"
#include "TileMap.h"

TileCharacter::TileCharacter(TileMap &tilemap, jl::AssetManager &assets, const sf::Vector2i &tileIndex) :
	m_tileMap(&tilemap),
	m_animation(),
	m_tileIndex(tileIndex),
	m_sprite(),
	m_isWalking(false),
	m_maxHealth(0),
	m_health(0),
	m_direction(TileCharacter::IdleDown),
	m_currencyAmount(0),
	m_ammoAmount(0)
{
	m_sprite.setPosition(
		tilemap.getTileSize()*tileIndex.x,
		tilemap.getTileSize()*tileIndex.y);
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

bool TileCharacter::affordCurrency(int currencyCost)
{
	return (m_currencyAmount - currencyCost) >= 0;
}
bool TileCharacter::affordAmmo(int ammoCost)
{
	return (m_ammoAmount - ammoCost) >= 0;
}
void TileCharacter::addCurrency(int currency)
{
	m_currencyAmount += currency;
	if(m_currencyAmount > TileCharacter::maxCurrency)
		m_currencyAmount = TileCharacter::maxCurrency;
}
void TileCharacter::addAmmo(int ammo)
{
	m_ammoAmount += ammo;
	if(m_ammoAmount > TileCharacter::maxAmmo)
		m_ammoAmount = TileCharacter::maxAmmo;
}
void TileCharacter::removeCurrency(int currency)
{
	m_currencyAmount -= currency;
		if(m_currencyAmount < 0)
			m_currencyAmount = 0;
}
void TileCharacter::removeAmmo(int ammo)
{
	m_ammoAmount -= ammo;
	if(m_ammoAmount < 0)
		m_ammoAmount = 0;
}
void TileCharacter::damage(int damage)
{
	m_health -= damage;
}

void TileCharacter::walkRight()
{
	if(!isWalking())
	{
		int x = m_tileIndex.x + 1, y = m_tileIndex.y;
		if(x < m_tileMap->getMapSize().x)
		{
			if(!m_tileMap->getTile(x, y).isOccupied() &&
			   !m_tileMap->getTile(x, y).isSolid())
			{
				// Deoccupy previous tile
				m_tileMap->getTile(m_tileIndex.x, y).clearCharacter();
				// Occupy new tile
				++m_tileIndex.x;
				m_tileMap->getTile(m_tileIndex.x, y).setCharacter(this);
				
				// Set walking data
				setWalking(true);
				m_direction = TileCharacter::WalkRight;
			}
		}
	}
}
void TileCharacter::walkLeft()
{
	if(!isWalking())
	{
		int x = m_tileIndex.x - 1, y = m_tileIndex.y;
		if(x >= 0)
		{
			if(!m_tileMap->getTile(x, y).isOccupied() &&
			   !m_tileMap->getTile(x, y).isSolid())
			{
				// Deoccupy previous tile
				m_tileMap->getTile(m_tileIndex.x, y).clearCharacter();
				// Occupy new tile
				--m_tileIndex.x;
				m_tileMap->getTile(m_tileIndex.x, y).setCharacter(this);
							
				// Set walking data
				setWalking(true);
				m_direction = TileCharacter::WalkLeft;
			}
		}
	}
}
void TileCharacter::walkUp()
{
	if(!isWalking())
	{
		int x = m_tileIndex.x, y = m_tileIndex.y - 1;
		if(y >= 0)
		{
			if(!m_tileMap->getTile(x, y).isOccupied() &&
			   !m_tileMap->getTile(x, y).isSolid())
			{
				// Deoccupy previous tile
				m_tileMap->getTile(x, m_tileIndex.y).clearCharacter();
				// Occupy new tile
				--m_tileIndex.y;

				m_tileMap->getTile(x, m_tileIndex.y).setCharacter(this);
							
				// Set walking data
				setWalking(true);
				m_direction = TileCharacter::WalkUp;
			}
		}
	}
}
void TileCharacter::walkDown()
{
	if(!isWalking())
	{
		int x = m_tileIndex.x, y = m_tileIndex.y + 1;
		if(y < m_tileMap->getMapSize().y)
		{
			if(!m_tileMap->getTile(x, y).isOccupied() &&
			   !m_tileMap->getTile(x, y).isSolid())
			{
				// Deoccupy previous tile
				m_tileMap->getTile(x, m_tileIndex.y).clearCharacter();
				// Occupy new tile
				++m_tileIndex.y;
				m_tileMap->getTile(x, m_tileIndex.y).setCharacter(this);
							
				// Set walking data
				setWalking(true);
				m_direction = TileCharacter::WalkDown;
			}
		}
	}
}

bool TileCharacter::lookingRight()
{
	return m_direction == TileCharacter::WalkRight || m_direction == TileCharacter::IdleRight;
}
bool TileCharacter::lookingLeft()
{
	return m_direction == TileCharacter::WalkLeft || m_direction == TileCharacter::IdleLeft;
}
bool TileCharacter::lookingUp()
{
	return m_direction == TileCharacter::WalkUp || m_direction == TileCharacter::IdleUp;
}
bool TileCharacter::lookingDown()
{
	return m_direction == TileCharacter::WalkDown || m_direction == TileCharacter::IdleDown;
}

sf::Sprite &TileCharacter::getSprite()
{
	return m_sprite;
}
jl::FrameAnimation &TileCharacter::getAnim()
{
	return m_animation;
}
TileMap &TileCharacter::getTileMap()
{
	return *m_tileMap;
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
int TileCharacter::getCurrency() const
{
	return m_currencyAmount;
}
int TileCharacter::getAmmo() const
{
	return m_ammoAmount;
}

bool TileCharacter::isWalking() const
{
	return m_isWalking;
}
bool TileCharacter::isDead() const
{
	return m_health <= 0;
}