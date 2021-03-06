#include "Player.h"
#include "TileMap.h"
#include "MessageLog.h"
#include "Utility.h"
#include "GunWeapon.h"
#include "RifleWeapon.h"
#include "Settings.h"
#include "GameState.h"
#include "SoundManager.h"
#include "XboxInput.h"

Player::Player(const std::string &name, GameState *gameState, jl::AssetManager &assets, const sf::Vector2i &tileIndex) :
	TileCharacter(name, gameState, assets, tileIndex),
	m_selectedWeapon(0),
	m_score(0)
{
	setSpeed(100);
	setMaxHealth(5);
	heal(5);
	gameState->getBed().setRegenDelay(2);

	m_animation.createAnimation("right");
	m_animation.pushFrame(sf::IntRect(0, 16, 16, 16), 0.1).
		pushFrame(sf::IntRect(16, 16, 16, 16), 0.1).
		pushFrame(sf::IntRect(32, 16, 16, 16), 0.1).
		pushFrame(sf::IntRect(48, 16, 16, 16), 0.1);
	m_animation.createAnimation("left");
	m_animation.pushFrame(sf::IntRect(0, 0, 16, 16), 0.1).
		pushFrame(sf::IntRect(16, 0, 16, 16), 0.1).
		pushFrame(sf::IntRect(32, 0, 16, 16), 0.1).
		pushFrame(sf::IntRect(48, 0, 16, 16), 0.1);
	m_animation.createAnimation("up");
	m_animation.pushFrame(sf::IntRect(0, 32, 16, 16), 0.1).
		pushFrame(sf::IntRect(16, 32, 16, 16), 0.1).
		pushFrame(sf::IntRect(32, 32, 16, 16), 0.1).
		pushFrame(sf::IntRect(48, 32, 16, 16), 0.1);
	m_animation.createAnimation("down");
	m_animation.pushFrame(sf::IntRect(0, 48, 16, 16), 0.1).
		pushFrame(sf::IntRect(16, 48, 16, 16), 0.1).
		pushFrame(sf::IntRect(32, 48, 16, 16), 0.1);
	m_animation.createAnimation("lookRight");
	m_animation.pushFrame(sf::IntRect(0, 16, 16, 16), 0.1);
	m_animation.createAnimation("lookLeft");
	m_animation.pushFrame(sf::IntRect(0, 0, 16, 16), 0.1);
	m_animation.createAnimation("lookUp");
	m_animation.pushFrame(sf::IntRect(0, 32, 16, 16), 0.1);
	m_animation.createAnimation("lookDown");
	m_animation.pushFrame(sf::IntRect(0, 48, 16, 16), 0.1);

	m_animation.createAnimation("sleep");
	m_animation.pushFrame(sf::IntRect(0, 64, 16, 16), 0.1).
		pushFrame(sf::IntRect(16, 64, 16, 16), 0.1).
		pushFrame(sf::IntRect(32, 64, 16, 16), 0.1).
		pushFrame(sf::IntRect(48, 64, 16, 16), 0.1).
		pushFrame(sf::IntRect(64, 64, 16, 16), 0.1).
		pushFrame(sf::IntRect(80, 64, 16, 16), 0.1);

	m_animation.initAnimation(m_sprite, "down");
	m_sprite.setTexture(assets.getTexture("res/rpgmaker16.png"));

	// Transparent color
	sf::Color transparentColor(sf::Color::White); 
	transparentColor.a = 200;



	// Set color of playerText to semi transparent to minimize gameplay interfering
	m_playerText.setFont(assets.getFont("res/Minecraftia.ttf"));
	m_playerText.setCharacterSize(30);
	m_playerText.setColor(transparentColor);

	m_resourceText.setFont(assets.getFont("res/Minecraftia.ttf"));
	m_resourceText.setCharacterSize(30);
	m_resourceText.setColor(transparentColor);
	
	m_materialSprite.setTexture(assets.getTexture("res/tiles.png"));
	m_materialSprite.setTextureRect(sf::IntRect(48, 0, 8, 7));
	m_materialSprite.setScale(5., 5.0);
	m_materialSprite.setColor(transparentColor);

	m_ammoSprite.setTexture(assets.getTexture("res/tiles.png"));
	m_ammoSprite.setTextureRect(sf::IntRect(49, 8, 6, 8));
	m_ammoSprite.setScale(5.0, 5.0);
	m_ammoSprite.setColor(transparentColor);

	m_healthSprite.setTexture(assets.getTexture("res/tiles.png"));
	m_healthSprite.setTextureRect(sf::IntRect(49, 17, 7, 7));
	m_healthSprite.setScale(5, 5);
	m_healthSprite.setColor(transparentColor);

	m_scoreSprite.setTexture(assets.getTexture("res/tiles.png"));
	m_scoreSprite.setTextureRect(sf::IntRect(57, 17, 7, 7));
	m_scoreSprite.setScale(5, 5);
	m_scoreSprite.setColor(transparentColor);
}

void Player::characterEvents(TileCharacter::Event events)
{
	if(m_gameState->getBed().isInUse())
		m_animation.request("sleep");
	else if(events == TileCharacter::WalkingRight)
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

void Player::events(sf::Event &events)
{
	if(!m_gameState->getBed().isInUse() && !m_gameState->getWorkbench().isVisible())
	{

		// Scroll through weapons
		bool changedWeapon = false;

		if(events.type == sf::Event::JoystickButtonPressed)
		{
			//if(jl::Input::isButtonDown(events, 2))
			if(jl::XboxInput::isButtonDown(0, jl::XboxInput::Buttons::X))
			{
				--m_selectedWeapon; 
				changedWeapon = true;
			}
			//if(jl::Input::isButtonDown(events, 3))
			else if(jl::XboxInput::isButtonDown(0, jl::XboxInput::Buttons::Y))
			{
				++m_selectedWeapon; 
				changedWeapon = true;
			}
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
	}
}

void Player::update(double deltaTime)
{
	/* SPRINTING
	if(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Z) == 100)
		setSpeed(150);
	else
		setSpeed(100);*/
	if(!isDead())
	{
		if(!m_gameState->getBed().isInUse() && !m_gameState->getWorkbench().isVisible())
		{
			// Fire weapon
			//if(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::R) == 100)
			if((jl::XboxInput::isAxisDown(0, jl::XboxInput::Axis::RightTrigger, 100, 50) && jl::XboxInput::usingUnix()) ||
				(jl::XboxInput::isAxisDown(0, jl::XboxInput::Axis::RightTrigger, -100, 50) && jl::XboxInput::usingWindows()))
				getActiveWeapon()->fire();

	       	// Changing direction of player
			//if(jl::Math::valueInRange<float, float, float>(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::V), -100, 25))
			if(jl::XboxInput::isAxisDown(0, jl::XboxInput::Axis::RightJoyY, -100, 25))
	     		lookUp();
			//else if(jl::Math::valueInRange<float, float, float>(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::V), 100, 25))
			else if(jl::XboxInput::isAxisDown(0, jl::XboxInput::Axis::RightJoyY, 100, 25))
	       		lookDown();
			//else if(jl::Math::valueInRange<float, float, float>(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::U), -100, 25))
			else if(jl::XboxInput::isAxisDown(0, jl::XboxInput::Axis::RightJoyX, -100, 25))
	       		lookLeft();
			//else if(jl::Math::valueInRange<float, float, float>(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::U), 100, 25))
			else if(jl::XboxInput::isAxisDown(0, jl::XboxInput::Axis::RightJoyX, 100, 25))
	       		lookRight();
	       	else
	       	{
		       	// Walking around
				//if(jl::Math::valueInRange<float, float, float>(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y), -100, 25))
				if(jl::XboxInput::isAxisDown(0, jl::XboxInput::Axis::LeftJoyY, -100, 25))
		     		walkUp();
				//else if(jl::Math::valueInRange<float, float, float>(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y), 100, 25))
				else if(jl::XboxInput::isAxisDown(0, jl::XboxInput::Axis::LeftJoyY, 100, 25))
		       		walkDown();
				//else if(jl::Math::valueInRange<float, float, float>(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X), -100, 25))
				else if(jl::XboxInput::isAxisDown(0, jl::XboxInput::Axis::LeftJoyX, -100, 25))
		       		walkLeft();
				//else if(jl::Math::valueInRange<float, float, float>(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X), 100, 25))
				else if(jl::XboxInput::isAxisDown(0, jl::XboxInput::Axis::LeftJoyX, 100, 25))
		       		walkRight();
	       	}

		}

		m_animation.commit(m_sprite, deltaTime);

		for(std::size_t i = 0; i < m_weapons.size(); i++)
			m_weapons[i]->updateBullets(deltaTime);

		getActiveWeapon()->updateWeapon(deltaTime);
	}
}
void Player::render(sf::RenderTarget &target)
{
	if(!isDead())
	{

		if(!m_gameState->isPaused())
		{

			sf::View tempView(target.getView());
			target.setView(target.getDefaultView());

			// Draw currency text
			m_resourceText.setPosition(target.getView().getSize().x * 0.1, target.getView().getSize().y * 0.8);
			m_resourceText.setString(jl::Util::toString(m_currencyAmount));
			target.draw(m_resourceText);
			// Draw currency icon
			m_materialSprite.setPosition(
				m_resourceText.getPosition().x - (m_materialSprite.getGlobalBounds().width+10),
				(m_resourceText.getPosition().y + m_resourceText.getGlobalBounds().height/2) - m_materialSprite.getGlobalBounds().height/2);
			target.draw(m_materialSprite);

			// Draw stored ammo text
			m_resourceText.setPosition(target.getView().getSize().x * 0.1, (target.getView().getSize().y * 0.8) + m_resourceText.getGlobalBounds().height*2);
			m_resourceText.setString(jl::Util::toString(m_ammoAmount));
			target.draw(m_resourceText);
			// Draw ammo icon
			m_ammoSprite.setPosition(
				m_resourceText.getPosition().x - (m_ammoSprite.getGlobalBounds().width+10),
				(m_resourceText.getPosition().y + m_resourceText.getGlobalBounds().height/2) - m_ammoSprite.getGlobalBounds().height/2);
			target.draw(m_ammoSprite);

			// Draw score
			m_resourceText.setPosition(target.getView().getSize().x * 0.1, (target.getView().getSize().y * 0.8) - m_resourceText.getGlobalBounds().height*2);
			m_resourceText.setString(jl::Util::toString(getScore()));
			target.draw(m_resourceText);
			// Draw score icon
			m_scoreSprite.setPosition(
				m_resourceText.getPosition().x - (m_scoreSprite.getGlobalBounds().width+10),
				(m_resourceText.getPosition().y + m_resourceText.getGlobalBounds().height/2) - m_scoreSprite.getGlobalBounds().height/2);
			target.draw(m_scoreSprite);

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
		}

		target.draw(m_sprite);
	

		// Render weapon bullets
		for(std::size_t i = 0; i < m_weapons.size(); i++)
			m_weapons[i]->renderBullets(target);

		// Render weapon
		if(!m_gameState->getBed().isInUse())
			getActiveWeapon()->renderWeapon(target);
	}
}

void Player::addWeapon(std::shared_ptr<Weapon>& weapon)
{
	m_weapons.push_back(weapon);
}
void Player::addScore(int score)
{
	m_score += score;
}

Weapon *Player::getActiveWeapon()
{
	return m_weapons[m_selectedWeapon].get();
}
int Player::getScore() const
{
	return m_score;
}
