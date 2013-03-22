#include "Player.h"
#include <iostream>

Player::Player(TileMap &tilemap) : TileCharacter(tilemap)
{
	setSpeed(100);
	setMaxHealth(5);

	m_animation.createAnimation("right");
	m_animation.pushFrame(sf::IntRect(0, 0, 16, 16), 0.5);
	m_animation.createAnimation("left");
	m_animation.pushFrame(sf::IntRect(16, 0, 16, 16), 0.5);
	m_animation.createAnimation("up");
	m_animation.pushFrame(sf::IntRect(32, 0, 16, 16), 0.5);
	m_animation.createAnimation("down");
	m_animation.pushFrame(sf::IntRect(48, 0, 16, 16), 0.5);

	m_animation.initAnimation(m_sprite, "down");
}

void Player::doWalkRight()
{
	m_animation.request("right");
}
void Player::doWalkLeft()
{
	m_animation.request("left");
}
void Player::doWalkUp()
{
	m_animation.request("up");
}
void Player::doWalkDown()
{
	m_animation.request("down");
}

void Player::events(sf::Event &events)
{

}
void Player::update(double deltaTime)
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !isWalking())
		walkUp();
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !isWalking())
		walkDown();
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !isWalking())
		walkLeft();
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !isWalking())
		walkRight();

	m_animation.commit(m_sprite, deltaTime);
}
void Player::render(sf::RenderTarget &target)
{
	target.draw(m_sprite);
}