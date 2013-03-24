#include "Weapon.h"
#include "TileMap.h"
#include "TileCharacter.h"

Weapon::Weapon(const std::string &name, TileCharacter *tileCharacter, jl::AssetManager &assets) :
	m_trackedCharacter(tileCharacter),
	m_activeStance(""),
	m_ammoCost(0),
	m_fireRate(0),
	m_bulletSpeed(0),
	m_name(name)
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
	if(m_fireRateClock.getElapsedTime().asSeconds() >= m_fireRate && m_trackedCharacter->affordAmmo(m_ammoCost))
	{
		m_trackedCharacter->removeAmmo(m_ammoCost);
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

void Weapon::update(double deltaTime)
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
		update(i, deltaTime);
	}

}
void Weapon::render(sf::RenderTarget &target)
{
	m_weaponSprite.setPosition(getWeaponPos());
	target.draw(m_weaponSprite);

	for(std::size_t i = 0; i < m_bullets.size(); i++)
		render(i, target);
}

std::string Weapon::getStance() const
{
	return m_activeStance;
}
std::string Weapon::getName() const
{
	return m_name;
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