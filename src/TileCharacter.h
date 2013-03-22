#ifndef TILECHARACTER_H
#define TILECHARACTER_H

#include <SFML/Graphics.hpp>
#include "FrameAnimation.h"

class TileMap;
class TileCharacter
{
public:
	enum Directions
	{
		Right = 0,
		Left = 1,
		Up = 2,
		Down = 3,
		IdleRight = 4,
		IdleLeft = 5,
		IdleUp = 6,
		IdleDown = 7
	};
private:

	virtual void doWalkRight(){};
	virtual void doWalkLeft(){};
	virtual void doWalkUp(){};
	virtual void doWalkDown(){};

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
	explicit TileCharacter(TileMap &tilemap);

public:

	virtual void events(sf::Event &events) = 0;
	virtual void update(double deltaTime) = 0;
	virtual void render(sf::RenderTarget &target) = 0;

	void setMaxHealth(int health);
	void setSpeed(double speed);
	void setWalking(bool walking);
	void setDirection(TileCharacter::Directions direction);
	void setIndex(std::size_t x, std::size_t y);
	void damage(int damage);

	void walkRight();
	void walkLeft();
	void walkUp();
	void walkDown();

	sf::Sprite &getSprite();
	jl::FrameAnimation &getAnim();
	sf::Vector2i getIndex() const;
	TileCharacter::Directions getDirection() const;
	double getSpeed() const;
	bool isWalking() const;
	bool isDead() const;
};

#endif