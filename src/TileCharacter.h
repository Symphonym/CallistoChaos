#ifndef TILECHARACTER_H
#define TILECHARACTER_H

#include <SFML/Graphics.hpp>
#include "FrameAnimation.h"
#include "AssetManager.h"


class TileMap;
class TileCharacter
{
public:
	enum Event
	{
		WalkingRight = 0,
		WalkingLeft = 1,
		WalkingUp = 2,
		WalkingDown = 3,
		LookingRight = 4,
		LookingLeft = 5,
		LookingUp = 6,
		LookingDown = 7,
		GoRight = 8,
		GoLeft = 9,
		GoUp = 10,
		GoDown = 11
	};

protected:
	TileMap *m_tileMap;
	jl::FrameAnimation m_animation;
	sf::Vector2i m_tileIndex;
	sf::Sprite m_sprite;
	bool m_isWalking, m_isBusy;
	int m_maxHealth, m_health;
	double m_speed;
	// Looking/Walking direction
	Event m_direction;
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
	static const int maxCurrency = 999;
	// Maximum ammo
	static const int maxAmmo = 999;

	virtual void events(sf::Event &events) = 0;
	virtual void update(double deltaTime) = 0;
	virtual void render(sf::RenderTarget &target) = 0;

	virtual void characterEvents(TileCharacter::Event events){};

	void setMaxHealth(int health);
	void setSpeed(double speed);
	void setBusy(bool busy);
	void setWalking(bool walking);
	void setDirection(TileCharacter::Event direction);

	// Checks if the Character can afford the currency cost
	bool affordCurrency(int currencyCost);
	// Check if the Character can afford the ammo cost
	bool affordAmmo(int ammoCost);
	void addCurrency(int currency);
	void addAmmo(int ammo);
	void removeCurrency(int currency);
	void removeAmmo(int ammo);

	void damage(int damage);
	void heal(int healing);

	void walkRight();
	void walkLeft();
	void walkUp();
	void walkDown();

	void lookRight();
	void lookLeft();
	void lookUp();
	void lookDown();

	bool lookingRight() const;
	bool lookingLeft() const;
	bool lookingUp() const;
	bool lookingDown() const;

	sf::Sprite &getSprite();
	jl::FrameAnimation &getAnim();
	TileMap &getTileMap();
	sf::Vector2i getIndex() const;
	TileCharacter::Event getDirection() const;
	double getSpeed() const;	
	int getCurrency() const;
	int getAmmo() const;
	bool isBusy() const;
	bool isWalking() const;
	bool isDead() const;
	bool isFullHealth() const;
};

#endif