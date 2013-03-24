#include "Player.h"
#include "TileMap.h"
#include "MessageLog.h"
#include "Utility.h"
#include "GunWeapon.h"

Player::Player(TileMap &tilemap, jl::AssetManager &assets, const sf::Vector2i &tileIndex) :
	TileCharacter(tilemap, assets, tileIndex),
	m_currencyAmount(100),
	m_ammoAmount(100)
{
	setSpeed(100);
	setMaxHealth(5);

	m_animation.createAnimation("right");
	m_animation.pushFrame(sf::IntRect(0, 32, 16, 16), 0.1).
		pushFrame(sf::IntRect(16, 32, 16, 16), 0.1).
		pushFrame(sf::IntRect(32, 32, 16, 16), 0.1);
	m_animation.createAnimation("left");
	m_animation.pushFrame(sf::IntRect(0, 16, 16, 16), 0.1).
		pushFrame(sf::IntRect(16, 16, 16, 16), 0.1).
		pushFrame(sf::IntRect(32, 16, 16, 16), 0.1);
	m_animation.createAnimation("up");
	m_animation.pushFrame(sf::IntRect(0, 48, 16, 16), 0.1).
		pushFrame(sf::IntRect(16, 48, 16, 16), 0.1).
		pushFrame(sf::IntRect(32, 48, 16, 16), 0.1);
	m_animation.createAnimation("down");
	m_animation.pushFrame(sf::IntRect(0, 0, 16, 16), 0.1).
		pushFrame(sf::IntRect(16, 0, 16, 16), 0.1).
		pushFrame(sf::IntRect(32, 0, 16, 16), 0.1);

	m_animation.initAnimation(m_sprite, "down");
	m_sprite.setTexture(assets.getAsset<jl::TextureAsset>("res/rpgmaker16.png")->get());

	m_resourceText.setFont(assets.getAsset<jl::FontAsset>("res/Minecraftia.ttf")->get());
	m_resourceText.setCharacterSize(8);

	m_weapons.push_back(std::move(std::unique_ptr<Weapon>(new GunWeapon(this, assets))));
}

void Player::duringWalkRight()
{
	m_animation.request("right");
}
void Player::duringWalkLeft()
{
	m_animation.request("left");
}
void Player::duringWalkUp()
{
	m_animation.request("up");
}
void Player::duringWalkDown()
{
	m_animation.request("down");
}
void Player::events(sf::Event &events)
{
	if(events.type == sf::Event::KeyPressed)
	{
		if(events.key.code == sf::Keyboard::C && m_direction == TileCharacter::IdleRight)
			m_weapons[0]->fire();
	}
	// Changing direction of player
	if(events.type == sf::Event::KeyPressed && !isWalking())
	{
		if(events.key.code == sf::Keyboard::Right)
		{
			setDirection(TileCharacter::IdleRight);
			m_animation.request("right");
		}
		else if (events.key.code == sf::Keyboard::Left)
		{
			setDirection(TileCharacter::IdleLeft);
			m_animation.request("left");
		}
		else if (events.key.code == sf::Keyboard::Up)
		{
			setDirection(TileCharacter::IdleUp);
			m_animation.request("up");
		}
		else if (events.key.code == sf::Keyboard::Down)
		{
			setDirection(TileCharacter::IdleDown);
			m_animation.request("down");
		}
	}
}

void Player::update(double deltaTime)
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		walkUp();
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		walkDown();
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		walkLeft();
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		walkRight();

	m_animation.commit(m_sprite, deltaTime);

	for(std::size_t i = 0; i < m_weapons.size(); i++)
		m_weapons[i]->update(deltaTime);
}
void Player::render(sf::RenderTarget &target)
{
	target.draw(m_sprite);

	sf::View tempView(target.getView());
	target.setView(target.getDefaultView());

	// Draw currency text
	m_resourceText.setPosition(331, 305);
	m_resourceText.setString(jl::Util::toString(m_currencyAmount));
	target.draw(m_resourceText);

	// Draw ammo text
	m_resourceText.setPosition(371, 305);
	m_resourceText.setString(jl::Util::toString(m_ammoAmount));
	target.draw(m_resourceText);

	target.setView(tempView);

	for(std::size_t i = 0; i < m_weapons.size(); i++)
		m_weapons[i]->render(target);
}


bool Player::pay(int currency)
{
	if(m_currencyAmount - currency >= 0)
	{
		m_currencyAmount -= currency;
		return true;
	}
	else
		return false;
}