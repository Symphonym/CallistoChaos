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
	Directions m_direction;

	// Make it an abstract class by making constructor protected
	// Pass Tilemap to allow for move operations
	// Pass AssetManager to allow sprite/animation data to be set in the constructor
	// Pass Tileindex to specify where the Character should be created
	explicit TileCharacter(TileMap &tilemap, jl::AssetManager &assets, const sf::Vector2i &tileIndex);

public:

	virtual void events(sf::Event &events) = 0;
	virtual void update(double deltaTime) = 0;
	virtual void render(sf::RenderTarget &target) = 0;

	void setMaxHealth(int health);
	void setSpeed(double speed);
	void setWalking(bool walking);
	void setDirection(TileCharacter::Directions direction);
	void damage(int damage);

	void walkRight();
	void walkLeft();
	void walkUp();
	void walkDown();

	virtual void duringWalkRight(){};
	virtual void duringWalkLeft(){};
	virtual void duringWalkUp(){};
	virtual void duringWalkDown(){};

	virtual void duringIdleRight(){};
	virtual void duringIdleLeft(){};
	virtual void duringIdleUp(){};
	virtual void duringIdleDown(){};

	sf::Sprite &getSprite();
	jl::FrameAnimation &getAnim();
	TileMap &getTileMap();
	sf::Vector2i getIndex() const;
	TileCharacter::Directions getDirection() const;
	double getSpeed() const;
	bool isWalking() const;
	bool isDead() const;
};

#endif