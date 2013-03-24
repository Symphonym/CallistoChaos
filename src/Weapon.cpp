#include "Weapon.h"

Weapon::Weapon(TileCharacter *tileCharacter, jl::AssetManager &assets) :
	m_trackedCharacter(tileCharacter),
	m_activeStance(""),
	m_ammoCost(0),
	m_fireRate(0),
	m_bulletSpeed(0)
{

}

void Weapon::addStance(const std::string &name, const sf::Vector2f &pos, const sf::IntRect &subRect, double rotation)
{
	StanceData stance;
	stance.position = pos;
	stance.rotation = rotation;
	stance.subRect = subRect;
	m_stances[name] = stance;
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
void Weapon::setAmmoCost(int cost)
{
	m_ammoCost = cost;
}
void Weapon::setFireRate(double delay)
{
	m_fireRate = delay;
}
void Weapon::setWeaponSheet(const sf::Texture &texture, const sf::Texture &bulletSheet)
{
	m_weaponSprite.setTexture(texture);
	m_bulletSprite.setTexture(bulletSheet);
}
void Weapon::setBulletSpeed(double speed)
{
	m_bulletSpeed = speed;
}
void Weapon::fire()
{
	if(m_fireRateClock.getElapsedTime().asSeconds() >= m_fireRate)
	{
		m_fireRateClock.restart();

		Weapon::BulletDirections direction = Weapon::Right;
		switch(m_trackedCharacter->getDirection())
		{
			case TileCharacter::WalkRight:
			case TileCharacter::IdleRight:
				direction = Weapon::Right;
			break;
			case TileCharacter::WalkLeft:
			case TileCharacter::IdleLeft:
				direction = Weapon::Left;
			break;
			case TileCharacter::WalkUp:
			case TileCharacter::IdleUp:
				direction = Weapon::Up;
			break;
			case TileCharacter::WalkDown:
			case TileCharacter::IdleDown:
				direction = Weapon::Down;
			break;
		}

		m_bullets.push_back(std::make_pair(m_trackedCharacter->getSprite().getPosition(), direction));
	}
}

std::string Weapon::getStance() const
{
	return m_activeStance;
}