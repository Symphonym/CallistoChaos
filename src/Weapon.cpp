#include "Weapon.h"
#include "TileMap.h"
#include "TileCharacter.h"
#include "Utility.h"
#include "MessageLog.h"
#include <sstream>
#include "Player.h"

Weapon::Weapon(const std::string &name, TileCharacter *tileCharacter, jl::AssetManager &assets) :
	m_name(name),
	m_activeStance(""),
	m_trackedCharacter(tileCharacter),
	m_bulletSheet(nullptr),
	m_ammoCost(0),
	m_ammo(0),
	m_maxAmmo(0),
	m_damage(0),
	m_upgradeLevel(1),
	m_fireRate(0.5),
	m_bulletSpeed(100),
	m_bulletSpread(0),
	m_knockBack(0,0)
{

}

void Weapon::addStance(const std::string &name, const sf::Vector2f &pos, const sf::IntRect &subRect, const sf::Vector2f &bulletPos, const sf::Vector2f &firePos, double rotation)
{
	StanceData stance;
	stance.position = pos;
	stance.rotation = rotation;
	stance.bulletPosition = bulletPos;
	stance.firePosition = firePos;
	stance.subRect = subRect;
	m_stances[name] = stance;
}
void Weapon::addBulletFireAnimation(const std::string &animationName)
{
	m_bulletFireAnimations.push_back(animationName);
}
void Weapon::addBulletAnimation(const std::string &animationName)
{
	m_bulletAnimations.push_back(animationName);
}
void Weapon::upgrade()
{
	m_upgradeLevel += 1;
}
int Weapon::reload(int ammo)
{
	m_ammo += ammo;

	if(m_ammo > calculateMaxAmmo())
	{
		int leftOvers = (m_ammo - calculateMaxAmmo());
		m_ammo -= leftOvers;
		return leftOvers;
	}
	else
		return 0;
}

void Weapon::setStance(const std::string &name, const sf::Vector2f &customPosition)
{
	m_activeStance = name;
	m_weaponSprite.setTextureRect(m_stances[name].subRect);
	m_weaponSprite.setOrigin(
		m_weaponSprite.getTextureRect().width/2,
		m_weaponSprite.getTextureRect().height/2);
	m_weaponSprite.setRotation(m_stances[name].rotation);
	
	if(customPosition == sf::Vector2f(0,0))
		m_weaponSprite.setPosition(getWeaponPos());
	else
		m_weaponSprite.setPosition(customPosition);;
}
void Weapon::setAmmo(int cost, int maxAmmo)
{
	m_ammoCost = cost;
	m_maxAmmo = maxAmmo;

	// Set unlimited ammo
	if(m_maxAmmo < 0)
		m_ammo = -1;
}
void Weapon::setDamage(int damage)
{
	m_damage = damage;
}
void Weapon::setFireRate(double delay)
{
	m_fireRate = delay;
}
void Weapon::setBulletSpread(double angle)
{
	m_bulletSpread = angle;
}
void Weapon::setKnockBack(const sf::Vector2f &knockBack)
{
	m_knockBack = knockBack;
}
void Weapon::setBulletSpeed(double speed)
{
	m_bulletSpeed = speed;
}
void Weapon::setWeaponSheet(const sf::Texture &texture, const sf::Texture &bulletSheet)
{
	m_weaponSprite.setTexture(texture);
	m_bulletSheet = &bulletSheet;
}
void Weapon::setBulletAnimation(const jl::FrameAnimation &animation)
{
	m_bulletAnimation = animation;
}

void Weapon::fire()
{
	if(m_fireRateClock.getElapsedTime().asSeconds() >= calculateFireRate())
	{		
		// Check if enough ammo, or if unlimited ammo was provided
		if((m_ammo - calculateCost()) >= 0 || calculateMaxAmmo() < 0)
		{

			m_ammo -= calculateCost();

			m_fireRateClock.restart();

			// Place bullet at weapon
			sf::Vector2f weaponPos(getWeaponPos());
			weaponPos.x += m_stances[m_activeStance].bulletPosition.x;
			weaponPos.y += m_stances[m_activeStance].bulletPosition.y;



			AnimatedSpriteData data;
			data.sprite.setTexture(*m_bulletSheet);
			data.animation = m_bulletAnimation;

			// Add bullet fire animation
			if(!m_bulletFireAnimations.empty())
			{
				sf::Vector2f firePos(getWeaponPos());
				firePos.x += m_stances[m_activeStance].firePosition.x;
				firePos.y += m_stances[m_activeStance].firePosition.y;


				AnimatedSpriteData fireData;
				fireData.sprite.setTexture(*m_bulletSheet);
				fireData.animation = m_bulletAnimation;
				fireData.animationName = m_bulletFireAnimations[std::rand() % m_bulletFireAnimations.size()];
				fireData.animation.initAnimation(fireData.sprite, fireData.animationName);
				fireData.sprite.setOrigin(
					fireData.sprite.getGlobalBounds().width/2,
					fireData.sprite.getGlobalBounds().height/2);
				fireData.sprite.setPosition(firePos);
				if(m_trackedCharacter->lookingRight())
					fireData.sprite.setRotation(90);
				else if(m_trackedCharacter->lookingLeft())
					fireData.sprite.setRotation(270);
				else if(m_trackedCharacter->lookingDown())
					fireData.sprite.setRotation(180);
				m_bulletFires.push_back(fireData);

			}
			if(!m_bulletAnimations.empty())
			{
				data.animationName = m_bulletAnimations[std::rand() % m_bulletAnimations.size()];
				data.animation.initAnimation(data.sprite, data.animationName);
			}

			data.sprite.setOrigin(
				data.sprite.getGlobalBounds().width/2,
				data.sprite.getGlobalBounds().height/2);
			data.sprite.setPosition(weaponPos);

			// Calculate bulletspread angle
			int angle = 0;
			if(m_bulletSpread != 0)
			{
				// Equation provided by Lukas Hagman
				angle = -m_bulletSpread+((double)std::rand() / (double)RAND_MAX)*m_bulletSpread*2;	
			}

			sf::Vector2f knockBack(m_knockBack);
			if(m_trackedCharacter->isWalking())
			{
				knockBack.x /=2;
				knockBack.y /=2;
			}

			switch(m_trackedCharacter->getDirection())
			{
				case TileCharacter::WalkingRight:
				case TileCharacter::LookingRight:
					data.sprite.setRotation(90 - angle);
					data.direction = sf::Vector2f(
						std::sin(jl::Math::degToRad<double>(90 + angle)),
						std::cos(jl::Math::degToRad<double>(90 + angle)));
					m_weaponSprite.move(-knockBack.x, 0);
				break;
				case TileCharacter::WalkingLeft:
				case TileCharacter::LookingLeft:
					data.sprite.setRotation(270 - angle);
					data.direction = sf::Vector2f(
						std::sin(jl::Math::degToRad<double>(270 + angle)),
						std::cos(jl::Math::degToRad<double>(270 + angle)));
					m_weaponSprite.move(knockBack.x, 0);
				break;
				case TileCharacter::WalkingUp:
				case TileCharacter::LookingUp:
					data.sprite.setRotation(angle);
					data.direction = sf::Vector2f(
						std::sin(jl::Math::degToRad<double>(angle)),
						-std::cos(jl::Math::degToRad<double>(angle)));
					m_weaponSprite.move(0, knockBack.y);
				break;
				case TileCharacter::WalkingDown:
				case TileCharacter::LookingDown:
					data.sprite.setRotation(180 + angle);
					data.direction = sf::Vector2f(
						std::sin(jl::Math::degToRad<double>(180 + angle)),
						-std::cos(jl::Math::degToRad<double>(180 + angle)));
					m_weaponSprite.move(0, -knockBack.y);
				break;
			}
			m_bullets.push_back(data);
		}
		else
			MessageLog::addSingleMessage(getName() +  " is out of ammunition");
	}
}

void Weapon::updateBullet(AnimatedSpriteData &bullet, double deltaTime)
{
	bullet.sprite.move(
		bullet.direction.x*calculateSpeed()*deltaTime,
		bullet.direction.y*calculateSpeed()*deltaTime);
}
void Weapon::renderBullet(AnimatedSpriteData &bullet, sf::RenderTarget &target)
{
	target.draw(bullet.sprite);
}

void Weapon::updateBullets(double deltaTime)
{
	for(std::size_t i = 0; i < m_bulletFires.size(); i++)
	{
		if(m_bulletFires[i].animation.hasPlayed())
		{
			m_bulletFires.erase(m_bulletFires.begin() + i);
			continue;
		}

		if(!m_bulletFires[i].animation.hasPlayed())
		{
			// Place fireAnimation at weapon
			sf::Vector2f firePos(getWeaponPos());
			firePos.x += m_stances[m_activeStance].firePosition.x;
			firePos.y += m_stances[m_activeStance].firePosition.y;
			m_bulletFires[i].sprite.setPosition(firePos);

			// Animate bullet fire
			m_bulletFires[i].animation.animate(m_bulletFires[i].sprite, m_bulletFires[i].animationName, deltaTime);
		}
	}
	for(std::size_t i = 0; i < m_bullets.size(); i++)
	{
		sf::Vector2i index(getBulletIndex(m_bullets[i]));
		Tile *tile = &m_trackedCharacter->getTileMap().getTile(index);

		// Check if bullet is outside map, delete bullet
		if(index.x < 0 || index.x >= m_trackedCharacter->getTileMap().getMapSize().x ||
			index.y < 0 || index.y >= m_trackedCharacter->getTileMap().getMapSize().y)
		{
			m_bullets.erase(m_bullets.begin() + i);
			continue;
		}

		// Collision with solid tile, delete bullet
		if(tile->isSolid() && tile->isPlayerAttackable())
		{	
			tile->damage(calculateDamage());
			m_bullets.erase(m_bullets.begin() + i);
			continue;
		}
		// Collision with enemy, delete bullet
		else if (tile->isOccupied() && !dynamic_cast<Player*>(tile->getCharacter()))
		{
			if(m_bullets[i].sprite.getGlobalBounds().intersects(tile->getCharacter()->getSprite().getGlobalBounds()))
			{
				tile->damage(calculateDamage());
				m_bullets.erase(m_bullets.begin() + i);
				continue;
			}
		}

		if(!m_bulletAnimations.empty())
			m_bullets[i].animation.animate(m_bullets[i].sprite, m_bullets[i].animationName, deltaTime);

		updateBullet(m_bullets[i], deltaTime);
	}

}
void Weapon::renderBullets(sf::RenderTarget &target)
{		
	for(std::size_t i = 0; i < m_bullets.size(); i++)
		renderBullet(m_bullets[i], target);
	for(std::size_t i = 0; i < m_bulletFires.size(); i++)
		target.draw(m_bulletFires[i].sprite);
}
void Weapon::updateWeapon(double deltaTime)
{
	m_weaponSprite.setPosition(jl::Vec::lerp(m_weaponSprite.getPosition(), getWeaponPos(), 40*deltaTime));
}
void Weapon::renderWeapon(sf::RenderTarget &target)
{		
	target.draw(m_weaponSprite);
}

std::string Weapon::getStance() const
{
	return m_activeStance;
}
std::string Weapon::getName() const
{
	return m_name;
}
int Weapon::getLevel() const
{
	return m_upgradeLevel;
}
int Weapon::getAmmo() const
{
	return m_ammo;
}
int Weapon::getMaxAmmo() const
{
	return m_maxAmmo;
}

bool Weapon::hasUnlimitedAmmo() const
{
	return m_ammo < 0;
}
bool Weapon::hasFullAmmo() const
{
	return m_ammo >= m_maxAmmo;
}



int Weapon::calculateDamage() const
{
	return m_damage;
}
int Weapon::calculateCost() const
{
	return m_ammoCost;
}
int Weapon::calculateMaxAmmo() const
{
	return m_maxAmmo;
}
double Weapon::calculateFireRate() const
{
	return m_fireRate;
}
double Weapon::calculateSpeed() const
{
	return m_bulletSpeed;
}


sf::Vector2i Weapon::getBulletIndex(const AnimatedSpriteData &bullet) const
{
	return sf::Vector2i(
		bullet.sprite.getPosition().x/m_trackedCharacter->getTileMap().getTileSize(),
		bullet.sprite.getPosition().y/m_trackedCharacter->getTileMap().getTileSize());
}
sf::Vector2f Weapon::getWeaponPos()
{
	return sf::Vector2f(
		m_trackedCharacter->getSprite().getPosition().x +
		(m_stances[m_activeStance].position.x + m_weaponSprite.getGlobalBounds().width/2),
		m_trackedCharacter->getSprite().getPosition().y +
		(m_stances[m_activeStance].position.y + m_weaponSprite.getGlobalBounds().height/2));

}
double Weapon::getSpeed(double deltaTime) const
{
	return calculateSpeed()*deltaTime;
}


std::string Weapon::toAmmoString()
{
	std::stringstream ss;
	std::string ammoString = "", maxAmmoString = "";

	if(m_maxAmmo < 0)
	{
		ammoString = "inf";
		maxAmmoString = "inf";
	}
	else
	{
		ss << m_ammo;
		ss >> ammoString;

		ss.clear();

		ss << m_maxAmmo;
		ss >> maxAmmoString;
	}

	return ammoString + "/" + maxAmmoString;
}