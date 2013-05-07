#include "Weapon.h"
#include "TileMap.h"
#include "TileCharacter.h"
#include "Utility.h"
#include "MessageLog.h"
#include <sstream>
#include "Player.h"
#include "ParticleManager.h"
#include "SoundManager.h"

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
	m_bulletLifetime(-1),
	m_customFire(false),
	m_fireSound(""),
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
void Weapon::setBulletLifetime(double lifetime)
{
	m_bulletLifetime = lifetime;
}
void Weapon::setCustomFire(bool custom)
{
	m_customFire = custom;
}
void Weapon::setFireSound(const std::string &soundPath)
{
	m_fireSound = soundPath;
	jl::SoundManager::addSound(soundPath);
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

			if(!hasUnlimitedAmmo())
				m_ammo -= calculateCost();

			m_fireRateClock.restart();

			// Place bullet at weapon
			sf::Vector2f weaponPos(getWeaponPos());
			weaponPos.x += m_stances[m_activeStance].bulletPosition.x;
			weaponPos.y += m_stances[m_activeStance].bulletPosition.y;

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

			sf::Vector2f knockBack(m_knockBack);
			if(m_trackedCharacter->isWalking())
			{
				knockBack.x /=2;
				knockBack.y /=2;
			}

			// Base directions for up and down are inverted to get the intended behaviour, idk why
			if(m_trackedCharacter->lookingRight())
				m_weaponSprite.move(-knockBack.x, 0);
			else if(m_trackedCharacter->lookingLeft())
				m_weaponSprite.move(knockBack.x, 0);
			else if(m_trackedCharacter->lookingUp())
				m_weaponSprite.move(0, knockBack.y);
			else if(m_trackedCharacter->lookingDown())
				m_weaponSprite.move(0, -knockBack.y);

			if(!m_fireSound.empty())
				jl::SoundManager::playSound(m_fireSound);	

			if(!m_customFire)
				spawnBullet(weaponPos);
		}
		else
			MessageLog::addSingleMessage(getName() +  " is out of ammunition");
	}
}
void Weapon::spawnBullet(const sf::Vector2f &position, double initialLifeTime)
{
	AnimatedSpriteData data;
	data.sprite.setTexture(*m_bulletSheet);
	data.animation = m_bulletAnimation;
	data.lifeTime = initialLifeTime;

	// Randomly select one of the bullet animations
	if(!m_bulletAnimations.empty())
	{
		data.animationName = m_bulletAnimations[std::rand() % m_bulletAnimations.size()];
		data.animation.initAnimation(data.sprite, data.animationName);
	}

	data.sprite.setOrigin(
		data.sprite.getGlobalBounds().width/2,
		data.sprite.getGlobalBounds().height/2);
	data.sprite.setPosition(position);

	// Calculate bulletspread angle
	int angle = 0;
	if(calculateBulletSpread() != 0)
	{
		// Equation provided by Lukas Hagman
		angle = -calculateBulletSpread()+((double)std::rand() / (double)RAND_MAX)*calculateBulletSpread()*2;	
	}

	// Base directions for up and down are inverted to get the intended behaviour, idk why
	if(m_trackedCharacter->lookingRight())
	{
			data.sprite.setRotation(90 - angle);
			data.baseDirection = 90;
			data.direction = sf::Vector2f(
				std::sin(jl::Math::degToRad<double>(90 + angle)),
				std::cos(jl::Math::degToRad<double>(90 + angle)));
	}
	else if(m_trackedCharacter->lookingLeft())
	{

			data.sprite.setRotation(270 - angle);
			data.baseDirection = 270; 
			data.direction = sf::Vector2f(
				std::sin(jl::Math::degToRad<double>(270 + angle)),
				std::cos(jl::Math::degToRad<double>(270 + angle)));
	}
	else if(m_trackedCharacter->lookingUp())
	{
			data.sprite.setRotation(angle);
			data.baseDirection = 180;
			data.direction = sf::Vector2f(
				std::sin(jl::Math::degToRad<double>(angle)),
				-std::cos(jl::Math::degToRad<double>(angle)));
	}
	else if(m_trackedCharacter->lookingDown())
	{
			data.sprite.setRotation(180 + angle);
			data.baseDirection = 0;
			data.direction = sf::Vector2f(
				std::sin(jl::Math::degToRad<double>(180 + angle)),
				-std::cos(jl::Math::degToRad<double>(180 + angle)));
	}

	m_bullets.push_back(data);
}

void Weapon::updateBullet(std::vector<Weapon::AnimatedSpriteData> &bullets, int i, double deltaTime)
{

	sf::Vector2i index(getBulletIndex(bullets[i]));
	Tile *tile = &getTrackedChar().getTileMap().getTile(index);

	bullets[i].sprite.move(
		bullets[i].direction.x*calculateSpeed()*deltaTime,
		bullets[i].direction.y*calculateSpeed()*deltaTime);

	// Collision with solid tile, delete bullet
	if(tile->isSolid() && tile->isPlayerAttackable())
	{	
		tile->damage(calculateDamage(), &getTrackedChar().getTileMap(), index, bullets[i].baseDirection);
		bullets.erase(bullets.begin() + i);
	}
	// Collision with enemy, delete bullet
	else if (tile->isOccupied() && tile->isOccupied() && getTrackedChar().getIndex() != index)
	{
		const sf::FloatRect bulletBounds(
			bullets[i].sprite.getPosition().x, 
			bullets[i].sprite.getPosition().y,
			bullets[i].sprite.getGlobalBounds().width/2, 
			bullets[i].sprite.getGlobalBounds().height/2);

		if(bulletBounds.intersects(tile->getCharacter()->getSprite().getGlobalBounds()))
		{
			tile->damage(calculateDamage(), &getTrackedChar().getTileMap(), index, bullets[i].baseDirection);
			bullets.erase(bullets.begin() + i);
		}
	}
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
			// Register that a bulletfire animation has ended to the user
			bulletFireCallBack();
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

		// Add to lifetime variable
		m_bullets[i].lifeTime += deltaTime;

		// Check if bullet is outside map, delete bullet
		if(index.x < 0 || index.x >= m_trackedCharacter->getTileMap().getMapSize().x ||
			index.y < 0 || index.y >= m_trackedCharacter->getTileMap().getMapSize().y)
		{
			m_bullets.erase(m_bullets.begin() + i);
			continue;
		}

		// Check if bullet is "dead", or expired
		if(m_bulletLifetime > 0 && m_bullets[i].lifeTime >= m_bulletLifetime)
		{
			m_bullets.erase(m_bullets.begin() + i);
			continue;
		}

		if(!m_bulletAnimations.empty())
			m_bullets[i].animation.animate(m_bullets[i].sprite, m_bullets[i].animationName, deltaTime);

		updateBullet(m_bullets, i, deltaTime);
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
	m_weaponSprite.setPosition(jl::Vec::lerp(m_weaponSprite.getPosition(), getWeaponPos(), 35*deltaTime));
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
	return calculateMaxAmmo();
}
TileCharacter &Weapon::getTrackedChar()
{
	return *m_trackedCharacter;
}
const Weapon::StanceData &Weapon::getActiveStance() const
{
	return m_stances.find(m_activeStance)->second;
}
double Weapon::getBulletLifetime() const
{
	return m_bulletLifetime;
}

bool Weapon::hasUnlimitedAmmo() const
{
	return m_ammo < 0;
}
bool Weapon::hasFullAmmo() const
{
	return m_ammo >= calculateMaxAmmo();
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
double Weapon::calculateBulletSpread() const
{
	return m_bulletSpread;
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
	std::string ammoString(jl::Util::toString(getAmmo()));
	std::string maxAmmoString(jl::Util::toString(getMaxAmmo()));

	if(hasUnlimitedAmmo())
	{
		ammoString = "inf";
		maxAmmoString = "inf";
	}

	return ammoString + "/" + maxAmmoString;
}
