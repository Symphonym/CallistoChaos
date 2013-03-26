#ifndef TILECHARACTER_H
#define TILECHARACTER_H

#include <SFML/Graphics.hpp>
#include "FrameAnimation.h"
#include "AssetManager.h"

class TileMap;
class TileCharacter
{
public:
	enum Directions
	{
		WalkRight = 0,
		WalkLeft = 1,
		WalkUp = 2,
		WalkDown = 3,
		IdleRight = 4,
		IdleLeft = 5,
		IdleUp = 6,
		IdleDown = 7
	};

protected:
	TileMap *m_tileMap;
	jl::FrameAnimation m_animation;
	sf::Vector2i m_tileIndex;
	sf::Sprite m_sprite;
	bool m_isWalking;
	int m_maxHealth, m_health;
	double m_speed;
	// Looking/Walking direction
	Directions m_direction;
	// How much currency the character has
	int m_currencyAmount;
	// How much ammo the character has
	int m_ammoAmount;

	// Make it an abstract class by making constructor protected
	// Pass Tilemap to allow for move operations
	// Pass AssetManager to allow sprite/animation data to be set in the constructor
	// Pass Tileindex to specify where the Character should be created
	explicit TileCharacter(TileMap &tilemap, jl::AssetManager &assets, const sf::Vector2i &tileIndex);

public:

	// Maximum currency
	static const int maxCurrency = 300;
	// Maximum ammo
	static const int maxAmmo = 300;

	virtual void events(sf::Event &events) = 0;
	virtual void update(double deltaTime) = 0;
	virtual void render(sf::RenderTarget &target) = 0;

	virtual void duringWalkRight(){};
	virtual void duringWalkLeft(){};
	virtual void duringWalkUp(){};
	virtual void duringWalkDown(){};

	virtual void duringIdleRight(){};
	virtual void duringIdleLeft(){};
	virtual void duringIdleUp(){};
	virtual void duringIdleDown(){};

	void setMaxHealth(int health);
	void setSpeed(double speed);
	void setWalking(bool walking);
	void setDirection(TileCharacter::Directions direction);

	// Checks if the Character can afford the currency cost
	bool affordCurrency(int currencyCost);
	// Check if the Character can afford the ammo cost
	bool affordAmmo(int ammoCost);
	void addCurrency(int currency);
	void addAmmo(int ammo);
	void removeCurrency(int currency);
	void removeAmmo(int ammo);

	void damage(int damage);

	void walkRight();
	void walkLeft();
	void walkUp();
	void walkDown();

	bool lookingRight();
	bool lookingLeft();
	bool lookingUp();
	bool lookingDown();

	sf::Sprite &getSprite();
	jl::FrameAnimation &getAnim();
	TileMap &getTileMap();
	sf::Vector2i getIndex() const;
	TileCharacter::Directions getDirection() const;
	double getSpeed() const;	
	int getCurrency() const;
	int getAmmo() const;
	bool isWalking() const;
	bool isDead() const;
};

#endif