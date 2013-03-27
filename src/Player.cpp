#include "Player.h"
#include "TileMap.h"
#include "MessageLog.h"
#include "Utility.h"
#include "GunWeapon.h"
#include "RifleWeapon.h"

Player::Player(TileMap &tilemap, jl::AssetManager &assets, const sf::Vector2i &tileIndex) :
	TileCharacter(tilemap, assets, tileIndex),
	m_selectedWeapon(0),
	m_isResting(false)
{
	setSpeed(100);
	setMaxHealth(5);
	addAmmo(76);
	addCurrency(100);

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


	// Transparent color
	sf::Color transparentColor(sf::Color::White); 
	transparentColor.a = 150;

	// Set color of playerText to semi transparent to minimize gameplay interfering
	m_playerText.setFont(assets.getAsset<jl::FontAsset>("res/Minecraftia.ttf")->get());
	m_playerText.setCharacterSize(24);
	m_playerText.setColor(transparentColor);

	m_ammoSprite.setTexture(assets.getAsset<jl::TextureAsset>("res/tiles.png")->get());
	m_ammoSprite.setTextureRect(sf::IntRect(49, 8, 6, 8));
	m_ammoSprite.setScale(5, 5);
	m_ammoSprite.setColor(transparentColor);
	m_healthSprite.setTexture(assets.getAsset<jl::TextureAsset>("res/tiles.png")->get());
	m_healthSprite.setTextureRect(sf::IntRect(49, 17, 7, 7));
	m_healthSprite.setScale(5, 5);
	m_healthSprite.setColor(transparentColor);

	// Give player to becControl
	m_bedControl.providePlayer(this);

	m_weapons.push_back(std::move(std::unique_ptr<Weapon>(new GunWeapon("Plasma Gun", this, assets))));
	m_weapons.push_back(std::move(std::unique_ptr<Weapon>(new RifleWeapon("Pulse Rifle", this, assets))));
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

	if(events == TileCharacter::GoRight)
		getActiveWeapon()->setStance("right");
	else if(events == TileCharacter::GoLeft)
		getActiveWeapon()->setStance("left");
	else if(events == TileCharacter::GoUp)
		getActiveWeapon()->setStance("up");
	else if(events == TileCharacter::GoDown)
		getActiveWeapon()->setStance("down");
}

void Player::events(sf::Event &events)
{
	if(!m_bedControl.isInUse())
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
				turn(TileCharacter::LookingRight, true);
			else if(lookingLeft())
				turn(TileCharacter::LookingLeft, true);
			else if(lookingUp())
				turn(TileCharacter::LookingUp, true);
			else if(lookingDown())
				turn(TileCharacter::LookingDown, true);
		}

		// Changing direction of player
		if(!isWalking())
		{
			if(jl::Input::isKeyDown(events, sf::Keyboard::Right) || jl::Input::isAxisDown(events, sf::Joystick::Axis::U, 100))
				turn(TileCharacter::LookingRight);
			else if (jl::Input::isKeyDown(events, sf::Keyboard::Left) || jl::Input::isAxisDown(events, sf::Joystick::Axis::U, -100))
				turn(TileCharacter::LookingLeft);
			else if (jl::Input::isKeyDown(events, sf::Keyboard::Up) || jl::Input::isAxisDown(events, sf::Joystick::Axis::V, -100))
				turn(TileCharacter::LookingUp);
			else if (jl::Input::isKeyDown(events, sf::Keyboard::Down) || jl::Input::isAxisDown(events, sf::Joystick::Axis::V, 100))
				turn(TileCharacter::LookingDown);
		}
	}
}

void Player::update(double deltaTime)
{
	if(!m_bedControl.isInUse())
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

	for(std::size_t i = 0; i < m_weapons.size(); i++)
		m_weapons[i]->updateBullets(deltaTime);
}
void Player::render(sf::RenderTarget &target)
{
	sf::View tempView(target.getView());
	target.setView(target.getDefaultView());

	// Draw currency box text
	m_resourceText.setPosition(331, 305);
	m_resourceText.setString(jl::Util::toString(m_currencyAmount));
	target.draw(m_resourceText);

	// Draw ammo box text
	m_resourceText.setPosition(371, 305);
	m_resourceText.setString(jl::Util::toString(m_ammoAmount));
	target.draw(m_resourceText);

	// Draw player hp
	m_playerText.setPosition(250, target.getView().getSize().y - 50);
	m_playerText.setString(jl::Util::toString(m_health) + "/" + jl::Util::toString(m_maxHealth));
	target.draw(m_playerText);


	// Draw player weapon ammo
	m_playerText.setPosition(400, target.getView().getSize().y - 50);
	m_playerText.setString(getActiveWeapon()->toAmmoString());
	target.draw(m_playerText);

	// Draw ammo icon
	m_ammoSprite.setPosition(360, target.getView().getSize().y - 50);
	target.draw(m_ammoSprite);

	// Draw health icon
	m_healthSprite.setPosition(200, target.getView().getSize().y - 50);
	target.draw(m_healthSprite);

	target.setView(tempView);

	target.draw(m_sprite);


	// Render weapon bullets
	for(std::size_t i = 0; i < m_weapons.size(); i++)
		m_weapons[i]->renderBullets(target);

	// Render weapon
	m_weapons[m_selectedWeapon]->render(target);
}

void Player::sleepInBed(const sf::Vector2i &tileIndex)
{
	m_bedControl.toggleBed(tileIndex);
}
void Player::turn(TileCharacter::Event direction, bool weaponOnly)
{

	if(!isWalking())
	{
		sf::Vector2f playerCenter(
			m_sprite.getPosition().x + m_sprite.getGlobalBounds().width/2,
			m_sprite.getPosition().y + m_sprite.getGlobalBounds().height/2);

		if(!weaponOnly)
			setDirection(direction);

		if(lookingRight())
		{
			getActiveWeapon()->setStance("right", playerCenter);
			if(!weaponOnly)
			{
				m_animation.initAnimation(m_sprite, "right");
				m_animation.request("right");
			}
		}
		else if(lookingLeft())
		{
			getActiveWeapon()->setStance("left", playerCenter);
			if(!weaponOnly)
			{
				m_animation.initAnimation(m_sprite, "left");
				m_animation.request("left");
			}
		}
		else if(lookingUp())
		{
			getActiveWeapon()->setStance("up", playerCenter);
			if(!weaponOnly)
			{
				m_animation.initAnimation(m_sprite, "up");
				m_animation.request("up");
			}
		}
		else if(lookingDown())
		{
			getActiveWeapon()->setStance("down", playerCenter);
			if(!weaponOnly)
			{
				m_animation.initAnimation(m_sprite, "down");
				m_animation.request("down");
			}
		}
	}
}

Weapon *Player::getActiveWeapon()
{
	return m_weapons[m_selectedWeapon].get();
}