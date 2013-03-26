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
	if(!m_bedControl.isInUse())
	{

		// Scroll through weapons
		bool changedWeapon = false;

		if((events.mouseWheel.delta < 0 && events.type == sf::Event::MouseWheelMoved) || 
			(events.type == sf::Event::JoystickButtonPressed && events.joystickButton.button == 2))
		{
			--m_selectedWeapon; 
			changedWeapon = true;
		}
		if((events.mouseWheel.delta > 0 && events.type == sf::Event::MouseWheelMoved) || 
			(events.type == sf::Event::JoystickButtonPressed && events.joystickButton.button == 3))
		{
			++m_selectedWeapon; 
			changedWeapon = true;
		}

		if(m_selectedWeapon < 0)
			m_selectedWeapon = m_weapons.size() - 1;
		else if(m_selectedWeapon >= m_weapons.size())
			m_selectedWeapon = 0;

		if(changedWeapon)
			MessageLog::addMessage("Changed to weapon: " + getActiveWeapon()->getName());

		// Changing direction of player
		if((events.type == sf::Event::KeyPressed && !isWalking()) || 
			(events.type == sf::Event::JoystickMoved && !isWalking()))
		{
			if(events.key.code == sf::Keyboard::Right || 
				(events.joystickMove.axis == sf::Joystick::Axis::U && jl::Math::valueInRange<float, float, float>(events.joystickMove.position, 100, 25)))
			{
				setDirection(TileCharacter::IdleRight);
				m_animation.request("right");
			}
			else if (events.key.code == sf::Keyboard::Left || 
				(events.joystickMove.axis == sf::Joystick::Axis::U && jl::Math::valueInRange<float, float, float>(events.joystickMove.position, -100, 25)))
			{
				setDirection(TileCharacter::IdleLeft);
				m_animation.request("left");
			}
			else if (events.key.code == sf::Keyboard::Up || 
				(events.joystickMove.axis == sf::Joystick::Axis::V && jl::Math::valueInRange<float, float, float>(events.joystickMove.position, -100, 25)))
			{
				setDirection(TileCharacter::IdleUp);
				m_animation.request("up");
			}
			else if (events.key.code == sf::Keyboard::Down || 
				(events.joystickMove.axis == sf::Joystick::Axis::V && jl::Math::valueInRange<float, float, float>(events.joystickMove.position, 100, 25)))
			{
				setDirection(TileCharacter::IdleDown);
				m_animation.request("down");
			}
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

		if(lookingRight())
			getActiveWeapon()->setStance("right");
		else if(lookingLeft())
			getActiveWeapon()->setStance("left");
		else if(lookingUp())
			getActiveWeapon()->setStance("up");
		else if(lookingDown())
			getActiveWeapon()->setStance("down");

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

Weapon *Player::getActiveWeapon()
{
	return m_weapons[m_selectedWeapon].get();
}