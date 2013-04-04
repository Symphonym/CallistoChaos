#include "Player.h"
#include "TileMap.h"
#include "MessageLog.h"
#include "Utility.h"
#include "GunWeapon.h"
#include "RifleWeapon.h"
#include "Settings.h"

Player::Player(TileMap &tilemap, jl::AssetManager &assets, const sf::Vector2i &tileIndex) :
	TileCharacter(tilemap, assets, tileIndex),
	m_workbench(this, assets),
	m_bedControl(this),
	m_selectedWeapon(0)
{
	setSpeed(100);
	setMaxHealth(5);
	m_health = 2;
	addAmmo(100);
	addCurrency(999);
	m_bedControl.setRegenDelay(2);

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
	m_animation.createAnimation("lookRight");
	m_animation.pushFrame(sf::IntRect(0, 32, 16, 16), 0.1);
	m_animation.createAnimation("lookLeft");
	m_animation.pushFrame(sf::IntRect(0, 16, 16, 16), 0.1);
	m_animation.createAnimation("lookUp");
	m_animation.pushFrame(sf::IntRect(0, 48, 16, 16), 0.1);
	m_animation.createAnimation("lookDown");
	m_animation.pushFrame(sf::IntRect(0, 0, 16, 16), 0.1);

	m_animation.initAnimation(m_sprite, "down");
	m_sprite.setTexture(assets.getAsset<jl::TextureAsset>("res/rpgmaker16.png")->get());
	m_resourceText.setFont(assets.getAsset<jl::FontAsset>("res/Minecraftia.ttf")->get());
	m_resourceText.setCharacterSize(std::floor(8*jl::Settings::getDouble("gameRatio")));

	// Transparent color
	sf::Color transparentColor(sf::Color::White); 
	transparentColor.a = 150;

	// Set color of playerText to semi transparent to minimize gameplay interfering
	m_playerText.setFont(assets.getAsset<jl::FontAsset>("res/Minecraftia.ttf")->get());
	m_playerText.setCharacterSize(std::floor(30*jl::Settings::getDouble("gameRatio")));
	m_playerText.setColor(transparentColor);

	m_ammoSprite.setTexture(assets.getAsset<jl::TextureAsset>("res/tiles.png")->get());
	m_ammoSprite.setTextureRect(sf::IntRect(49, 8, 6, 8));
	m_ammoSprite.setScale(5.0*jl::Settings::getDouble("gameRatio"), 5.0*jl::Settings::getDouble("gameRatio"));
	m_ammoSprite.setColor(transparentColor);
	m_healthSprite.setTexture(assets.getAsset<jl::TextureAsset>("res/tiles.png")->get());
	m_healthSprite.setTextureRect(sf::IntRect(49, 17, 7, 7));
	m_healthSprite.setScale(5*jl::Settings::getDouble("gameRatio"), 5*jl::Settings::getDouble("gameRatio"));
	m_healthSprite.setColor(transparentColor);

	addWeapon(std::shared_ptr<Weapon>(new GunWeapon("Plasma Gun", this, assets)));
}

void Player::characterEvents(TileCharacter::Event events)
{
	if(events == TileCharacter::WalkingRight)
		m_animation.request("right");
	else if(events == TileCharacter::WalkingLeft)
		m_animation.request("left");
	else if(events == TileCharacter::WalkingUp)
		m_animation.request("up");
	else if(events == TileCharacter::WalkingDown)
		m_animation.request("down");

	else if(events == TileCharacter::LookingRight)
		m_animation.request("lookRight");
	else if(events == TileCharacter::LookingLeft)
		m_animation.request("lookLeft");
	else if(events == TileCharacter::LookingUp)
		m_animation.request("lookUp");
	else if(events == TileCharacter::LookingDown)
		m_animation.request("lookDown");

	if(events == TileCharacter::GoRight)
		getActiveWeapon()->setStance("right");
	else if(events == TileCharacter::GoLeft)
		getActiveWeapon()->setStance("left");
	else if(events == TileCharacter::GoUp)
		getActiveWeapon()->setStance("up");
	else if(events == TileCharacter::GoDown)
		getActiveWeapon()->setStance("down");
}
#include "AstarAlgorithm.h"
#include <iostream>
void Player::events(sf::Event &events)
{
	m_workbench.events(events);

	if(events.type == sf::Event::KeyReleased)
	{
		AstarAlgorithm::NodePath path = AstarAlgorithm::findPath(getIndex(), sf::Vector2i(0,0), &getTileMap());
		for(int i = 0; i < path.size(); i++)
		{
			getTileMap().getTile(path[i].index).setTileType(7);
			//std::cout << i << ": X(" << path[i].index.x << ") Y(" << path[i].index.y << ") " << std::endl;
		}
	}

	if(!m_bedControl.isInUse() && !m_workbench.isVisible())
	{
		// Scroll through weapons
		bool changedWeapon = false;

		if((jl::Input::scrollDirection(events) < 0) || jl::Input::isButtonDown(events, 2))
		{
			--m_selectedWeapon; 
			changedWeapon = true;
		}
		if((jl::Input::scrollDirection(events) > 0) || jl::Input::isButtonDown(events, 3))
		{
			++m_selectedWeapon; 
			changedWeapon = true;
		}

		if(m_selectedWeapon < 0)
			m_selectedWeapon = m_weapons.size() - 1;
		else if(m_selectedWeapon >= m_weapons.size())
			m_selectedWeapon = 0;

		if(changedWeapon)
		{
			sf::Vector2f playerCenter(
				m_sprite.getPosition().x + m_sprite.getGlobalBounds().width/2,
				m_sprite.getPosition().y + m_sprite.getGlobalBounds().height/2);

			MessageLog::addMessage("Changed to weapon: " + getActiveWeapon()->getName());
			if(lookingRight())
				getActiveWeapon()->setStance("right", playerCenter);
			else if(lookingLeft())
				getActiveWeapon()->setStance("left", playerCenter);
			else if(lookingUp())
				getActiveWeapon()->setStance("up", playerCenter);
			else if(lookingDown())
				getActiveWeapon()->setStance("down", playerCenter);
		}

		// Changing direction of player
		if(!isWalking())
		{
			if(jl::Input::isKeyDown(events, sf::Keyboard::Right) || jl::Input::isAxisDown(events, sf::Joystick::Axis::U, 100))
				lookRight();
			else if (jl::Input::isKeyDown(events, sf::Keyboard::Left) || jl::Input::isAxisDown(events, sf::Joystick::Axis::U, -100))
				lookLeft();
			else if (jl::Input::isKeyDown(events, sf::Keyboard::Up) || jl::Input::isAxisDown(events, sf::Joystick::Axis::V, -100))
				lookUp();
			else if (jl::Input::isKeyDown(events, sf::Keyboard::Down) || jl::Input::isAxisDown(events, sf::Joystick::Axis::V, 100))
				lookDown();
		}
	}
}

void Player::update(double deltaTime)
{
	/* SPRINTING
	if(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Z) == 100)
		setSpeed(150);
	else
		setSpeed(100);*/

	m_workbench.update(deltaTime);
	m_bedControl.update();

	if(!m_bedControl.isInUse() && !m_workbench.isVisible())
	{
		// Fire weapon
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::C) || sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::R) == 100)
			getActiveWeapon()->fire();

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) || 
    		jl::Math::valueInRange<float, float, float>(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y), -100, 25))
     		walkUp();
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) || 
     		jl::Math::valueInRange<float, float, float>(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y), 100, 25))
       		walkDown();
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) || 
	   		jl::Math::valueInRange<float, float, float>(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X), -100, 25))
       		walkLeft();
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) || 
	  		jl::Math::valueInRange<float, float, float>(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X), 100, 25))
       		walkRight();

		m_animation.commit(m_sprite, deltaTime);

	}

	for(auto it = m_weapons.begin(); it != m_weapons.end(); it++)
		it->second->updateBullets(deltaTime);

	getActiveWeapon()->updateWeapon(deltaTime);
}
void Player::render(sf::RenderTarget &target)
{

	sf::Vector2i currencyBoxPos(target.mapCoordsToPixel(
		sf::Vector2f(m_tileMap->getTilePosition(11, 10).x+4,m_tileMap->getTilePosition(11, 10).y+3), target.getView()));
	sf::Vector2i ammoBoxPos(target.mapCoordsToPixel(
		sf::Vector2f(m_tileMap->getTilePosition(12, 10).x+4,m_tileMap->getTilePosition(12, 10).y+3), target.getView()));

	sf::View tempView(target.getView());
	target.setView(target.getDefaultView());

	// Draw currency box text
	m_resourceText.setPosition(currencyBoxPos.x, currencyBoxPos.y);
	m_resourceText.setString(jl::Util::toString(m_currencyAmount));
	target.draw(m_resourceText);

	// Draw ammo box text
	m_resourceText.setPosition(ammoBoxPos.x, ammoBoxPos.y);
	m_resourceText.setString(jl::Util::toString(m_ammoAmount));
	target.draw(m_resourceText);

	// Draw player hp
	m_playerText.setPosition(target.getView().getSize().x * 0.3, target.getView().getSize().y * 0.9);
	m_playerText.setString(jl::Util::toString(m_health) + "/" + jl::Util::toString(m_maxHealth));
	target.draw(m_playerText);
	// Draw health icon
	m_healthSprite.setPosition(
		m_playerText.getPosition().x - m_healthSprite.getGlobalBounds().width*1.5,
		(m_playerText.getPosition().y + m_playerText.getGlobalBounds().height/2) - m_healthSprite.getGlobalBounds().height/2);
	target.draw(m_healthSprite);

	// Draw player weapon ammo
	m_playerText.setString(getActiveWeapon()->toAmmoString());
	m_playerText.setPosition(target.getView().getSize().x * 0.6, target.getView().getSize().y * 0.9);
	target.draw(m_playerText);
	// Draw ammo icon
	m_ammoSprite.setPosition(
		m_playerText.getPosition().x - m_ammoSprite.getGlobalBounds().width*1.5,
		(m_playerText.getPosition().y + m_playerText.getGlobalBounds().height/2) - m_ammoSprite.getGlobalBounds().height/2);
	target.draw(m_ammoSprite);

	target.setView(tempView);

	target.draw(m_sprite);


	// Render weapon bullets
	for(auto it = m_weapons.begin(); it != m_weapons.end(); it++)
		it->second->renderBullets(target);

	// Render weapon
	getActiveWeapon()->renderWeapon(target);

	m_workbench.render(target);
}

void Player::addWeapon(std::shared_ptr<Weapon> weapon)
{
	m_weapons[m_weapons.size()] = weapon;
}

Workbench &Player::getWorkbench()
{
	return m_workbench;
}
BedControl &Player::getBedControl()
{
	return m_bedControl;
}
Weapon *Player::getActiveWeapon()
{
	return m_weapons[m_selectedWeapon].get();
}