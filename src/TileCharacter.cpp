#include "TileCharacter.h"
#include "TileMap.h"
#include "GameState.h"

TileCharacter::TileCharacter(const std::string &name, GameState *gameState, jl::AssetManager &assets, const sf::Vector2i &tileIndex) :
	m_name(name),
	m_gameState(gameState),
	m_animation(),
	m_tileIndex(tileIndex),
	m_sprite(),
	m_isWalking(false),
	m_isBusy(false),
	m_maxHealth(0),
	m_health(0),
	m_direction(TileCharacter::LookingDown),
	m_currencyAmount(0),
	m_ammoAmount(0)
{
	m_sprite.setPosition(
		getTileMap().getTileSize()*tileIndex.x,
		getTileMap().getTileSize()*tileIndex.y);
}

void TileCharacter::setMaxHealth(int health)
{
	m_maxHealth = health;
}
void TileCharacter::setSpeed(double speed)
{
	m_speed = speed;
}
void TileCharacter::setWalking(bool walking)
{
	m_isWalking = walking;
}
void TileCharacter::setBusy(bool busy)
{
	m_isBusy = busy;
}
void TileCharacter::setDirection(TileCharacter::Event direction)
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

	if(m_health < 0)
		m_health = 0;
}
void TileCharacter::heal(int healing)
{
	m_health += healing;

	if(m_health > m_maxHealth)
		m_health = m_maxHealth;
}


void TileCharacter::walkRight()
{
	if(!isBusy())
	{
		int x = m_tileIndex.x + 1, y = m_tileIndex.y;
		if(x < getTileMap().getMapSize().x)
		{
			if(!getTileMap().getTile(x, y).isOccupied() &&
			   !getTileMap().getTile(x, y).isSolid())
			{
				// Deoccupy previous tile
				getTileMap().getTile(m_tileIndex.x, y).clearCharacter();
				// Occupy new tile
				++m_tileIndex.x;
				getTileMap().getTile(m_tileIndex.x, y).setCharacter(this);
				
				// Set walking data
				setWalking(true);
				m_direction = TileCharacter::WalkingRight;
				characterEvents(TileCharacter::GoRight);
			}
			else
				lookRight();
		}
	}
}
void TileCharacter::walkLeft()
{
	if(!isBusy())
	{
		int x = m_tileIndex.x - 1, y = m_tileIndex.y;
		if(x >= 0)
		{
			if(!getTileMap().getTile(x, y).isOccupied() &&
			   !getTileMap().getTile(x, y).isSolid())
			{
				// Deoccupy previous tile
				getTileMap().getTile(m_tileIndex.x, y).clearCharacter();
				// Occupy new tile
				--m_tileIndex.x;
				getTileMap().getTile(m_tileIndex.x, y).setCharacter(this);
							
				// Set walking data
				setWalking(true);
				m_direction = TileCharacter::WalkingLeft;
				characterEvents(TileCharacter::GoLeft);
			}
			else
				lookLeft();
		}
	}
}
void TileCharacter::walkUp()
{
	if(!isBusy())
	{
		int x = m_tileIndex.x, y = m_tileIndex.y - 1;
		if(y >= 0)
		{
			if(!getTileMap().getTile(x, y).isOccupied() &&
			   !getTileMap().getTile(x, y).isSolid())
			{
				// Deoccupy previous tile
				getTileMap().getTile(x, m_tileIndex.y).clearCharacter();
				// Occupy new tile
				--m_tileIndex.y;

				getTileMap().getTile(x, m_tileIndex.y).setCharacter(this);
							
				// Set walking data
				setWalking(true);
				m_direction = TileCharacter::WalkingUp;
				characterEvents(TileCharacter::GoUp);
			}
			else
				lookUp();
		}
	}
}
void TileCharacter::walkDown()
{
	if(!isBusy())
	{
		int x = m_tileIndex.x, y = m_tileIndex.y + 1;
		if(y < getTileMap().getMapSize().y)
		{
			if(!getTileMap().getTile(x, y).isOccupied() &&
			   !getTileMap().getTile(x, y).isSolid())
			{
				// Deoccupy previous tile
				getTileMap().getTile(x, m_tileIndex.y).clearCharacter();
				// Occupy new tile
				++m_tileIndex.y;
				getTileMap().getTile(x, m_tileIndex.y).setCharacter(this);
							
				// Set walking data
				setWalking(true);
				m_direction = TileCharacter::WalkingDown;
				characterEvents(TileCharacter::GoDown);
			}
			else
				lookDown();
		}
	}
}
void TileCharacter::lookRight()
{
	if(!isBusy())
	{
		m_direction = TileCharacter::LookingRight;
		characterEvents(TileCharacter::GoRight);
	}
}
void TileCharacter::lookLeft()
{
	if(!isBusy())
	{
		m_direction = TileCharacter::LookingLeft;
		characterEvents(TileCharacter::GoLeft);
	}
}
void TileCharacter::lookUp()
{
	if(!isBusy())
	{
		m_direction = TileCharacter::LookingUp;
		characterEvents(TileCharacter::GoUp);
	}
}
void TileCharacter::lookDown()
{
	if(!isBusy())
	{
		m_direction = TileCharacter::LookingDown;
		characterEvents(TileCharacter::GoDown);
	}
}

bool TileCharacter::lookingRight() const
{
	return m_direction == TileCharacter::WalkingRight || m_direction == TileCharacter::LookingRight;
}
bool TileCharacter::lookingLeft() const
{
	return m_direction == TileCharacter::WalkingLeft || m_direction == TileCharacter::LookingLeft;
}
bool TileCharacter::lookingUp() const
{
	return m_direction == TileCharacter::WalkingUp || m_direction == TileCharacter::LookingUp;
}
bool TileCharacter::lookingDown() const
{
	return m_direction == TileCharacter::WalkingDown || m_direction == TileCharacter::LookingDown;
}

GameState &TileCharacter::getGame()
{
	return *m_gameState;
}
TileMap &TileCharacter::getTileMap()
{
	return m_gameState->getTileMap();
}
CharacterManager &TileCharacter::getChars()
{
	return m_gameState->getChars();
}

sf::Sprite &TileCharacter::getSprite()
{
	return m_sprite;
}
jl::FrameAnimation &TileCharacter::getAnim()
{
	return m_animation;
}
const std::string &TileCharacter::getName() const
{
	return m_name;
}
sf::Vector2i TileCharacter::getIndex() const
{
	return m_tileIndex;
}
TileCharacter::Event TileCharacter::getDirection() const
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
int TileCharacter::getMaxHealth() const
{
	return m_maxHealth;
}
int TileCharacter::getHealth() const
{
	return m_health;
}

bool TileCharacter::isBusy() const
{
	return m_isWalking || m_isBusy;
}
bool TileCharacter::isWalking() const
{
	return m_isWalking;
}
bool TileCharacter::isDead() const
{
	return m_health <= 0;
}
bool TileCharacter::isFullHealth() const
{
	return m_health >= m_maxHealth;
}