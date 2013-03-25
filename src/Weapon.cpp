#include "Weapon.h"
#include "TileMap.h"
#include "TileCharacter.h"
#include "Utility.h"
#include <sstream>

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
	m_bulletSpeed(100)
{

}

void Weapon::addStance(const std::string &name, const sf::Vector2f &pos, const sf::IntRect &subRect, const sf::Vector2f &bulletPos, double rotation)
{
	StanceData stance;
	stance.position = pos;
	stance.rotation = rotation;
	stance.bulletPosition = bulletPos;
	stance.subRect = subRect;
	m_stances[name] = stance;
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

void Weapon::setStance(const std::string &name)
{
	m_activeStance = name;
	m_weaponSprite.setTextureRect(m_stances[name].subRect);
	m_weaponSprite.setOrigin(
		m_weaponSprite.getGlobalBounds().width/2,
		m_weaponSprite.getGlobalBounds().height/2);
	m_weaponSprite.setRotation(m_stances[name].rotation);
	m_weaponSprite.setPosition(
		m_trackedCharacter->getSprite().getPosition().x + m_stances[name].position.x + m_weaponSprite.getGlobalBounds().width/2,
		m_trackedCharacter->getSprite().getPosition().y + m_stances[name].position.y + + m_weaponSprite.getGlobalBounds().height/2);

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



			BulletData data;
			data.sprite.setTexture(*m_bulletSheet);
			data.animation = m_bulletAnimation;
			data.animation.initAnimation(data.sprite, "default");
			data.sprite.setOrigin(
				data.sprite.getGlobalBounds().width/2,
				data.sprite.getGlobalBounds().height/2);
			data.sprite.setPosition(weaponPos);

			switch(m_trackedCharacter->getDirection())
			{
				case TileCharacter::WalkRight:
				case TileCharacter::IdleRight:
					data.direction = Weapon::Right;
					data.sprite.setRotation(90);
				break;
				case TileCharacter::WalkLeft:
				case TileCharacter::IdleLeft:
					data.direction = Weapon::Left;
					data.sprite.setRotation(270);
				break;
				case TileCharacter::WalkUp:
				case TileCharacter::IdleUp:
					data.direction = Weapon::Up;
				break;
				case TileCharacter::WalkDown:
				case TileCharacter::IdleDown:
					data.direction = Weapon::Down;
					data.sprite.setRotation(180);
				break;
			}

			m_bullets.push_back(data);
		}
	}
}

void Weapon::update(BulletData &bullet, double deltaTime)
{
	sf::Vector2i index(getBulletIndex(bullet));
	switch(bullet.direction)
	{
		case Weapon::Right:
			bullet.sprite.move(getSpeed(deltaTime), 0);
		break;
		case Weapon::Left:
			bullet.sprite.move(-getSpeed(deltaTime), 0);
		break;
		case Weapon::Up:
			bullet.sprite.move(0, -getSpeed(deltaTime));
		break;
		case Weapon::Down:
			bullet.sprite.move(0, getSpeed(deltaTime));
		break;
	}

	bullet.animation.animate(bullet.sprite, "default", deltaTime);
}
void Weapon::render(BulletData &bullet, sf::RenderTarget &target)
{
	target.draw(bullet.sprite);
}
void Weapon::updateBullets(double deltaTime)
{
	for(std::size_t i = 0; i < m_bullets.size(); i++)
	{
		sf::Vector2i index(getBulletIndex(m_bullets[i]));

		// Check if bullet is outside map
		if(index.x < 0 || index.x >= m_trackedCharacter->getTileMap().getMapSize().x ||
			index.y < 0 || index.y >= m_trackedCharacter->getTileMap().getMapSize().y)
		{
			m_bullets.erase(m_bullets.begin() + i);
			continue;
		}

		Tile *tile = &m_trackedCharacter->getTileMap().getTile(index);

		// Collision with solid tile
		if(tile->isSolid() && tile->isPlayerAttackable())
		{	
			tile->damage(calculateDamage());
			m_bullets.erase(m_bullets.begin() + i);
			return;
		}


		update(m_bullets[i], deltaTime);
	}

}
void Weapon::renderBullets(sf::RenderTarget &target)
{
	for(std::size_t i = 0; i < m_bullets.size(); i++)
		render(m_bullets[i], target);
}
void Weapon::render(sf::RenderTarget &target)
{
	m_weaponSprite.setPosition(getWeaponPos());
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


sf::Vector2i Weapon::getBulletIndex(const BulletData &bullet) const
{
	return sf::Vector2i(
		bullet.sprite.getPosition().x/m_trackedCharacter->getTileMap().getTileSize(),
		bullet.sprite.getPosition().y/m_trackedCharacter->getTileMap().getTileSize());
}
sf::Vector2f Weapon::getWeaponPos()
{
	return sf::Vector2f(
		m_trackedCharacter->getSprite().getPosition().x +
		(m_stances[m_activeStance].position.x + m_stances[m_activeStance].subRect.width/2),
		m_trackedCharacter->getSprite().getPosition().y +
		(m_stances[m_activeStance].position.y + m_stances[m_activeStance].subRect.height/2));

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