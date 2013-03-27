#ifndef WEAPON_H
#define WEAPON_H

#include <SFML/Graphics.hpp>
#include <map>
#include "AssetManager.h"
#include "FrameAnimation.h"

class TileCharacter;
class Weapon
{
public:

	enum BulletDirections
	{
		Right = 0,
		Left = 1,
		Up = 2,
		Down = 3
	};

	struct StanceData
	{
		sf::Vector2f position, bulletPosition, firePosition;
		double rotation;
		sf::IntRect subRect;
	};
	struct BulletData
	{
		sf::Sprite sprite;
		sf::Vector2f direction;
		jl::FrameAnimation animation;
		std::string animationName;
	};
	struct BulletFireData
	{
		sf::Sprite sprite;
		jl::FrameAnimation animation;
		std::string animationName;
	};

private:
	
	// Name of the weapon
	std::string m_name;

	// Bullets
	std::vector<BulletData> m_bullets;
	std::vector<std::string> m_bulletAnimations;
	std::vector<BulletFireData> m_bulletFires;
	std::vector<std::string> m_bulletFireAnimations;

	jl::FrameAnimation m_bulletAnimation;

	// Different weapon stances
	std::map<std::string, StanceData> m_stances;
	std::string m_activeStance;

	// Character to shoot from
	TileCharacter *m_trackedCharacter;

	sf::Sprite m_weaponSprite;
	const sf::Texture *m_bulletSheet;


	int m_ammoCost, m_ammo, m_maxAmmo, m_damage, m_upgradeLevel;
	double m_fireRate, m_bulletSpeed, m_bulletSpread;
	sf::Vector2f m_knockBack;
	sf::Clock m_fireRateClock;

	virtual void update(BulletData &bullet, double deltaTime);
	virtual void render(BulletData &bullet, sf::RenderTarget &target);

protected:
	sf::Vector2i getBulletIndex(const BulletData &bullet) const;
	sf::Vector2f getWeaponPos();
	double getSpeed(double deltaTime) const;

	virtual int calculateDamage() const;
	virtual int calculateCost() const;
	virtual int calculateMaxAmmo() const;
	virtual double calculateFireRate() const;
	virtual double calculateSpeed() const;


public:

	explicit Weapon(const std::string &name, TileCharacter *tileCharacter, jl::AssetManager &assets);

	// Add a stance, position is relative to the tracked targe
	void addStance(const std::string &name, const sf::Vector2f &pos, const sf::IntRect &subRect, const sf::Vector2f &bulletPos = sf::Vector2f(0,0), const sf::Vector2f &firePos = sf::Vector2f(0,0), double rotation = 0);
	// Add bullet firing animation
	void addBulletFireAnimation(const std::string &animationName);
	// Add bullet animation
	void addBulletAnimation(const std::string &animationName);
	void upgrade();
	// Puts ammo into weapon, returns leftovers
	int reload(int ammo);

	void setStance(const std::string &name, const sf::Vector2f &customPosition = sf::Vector2f(0,0));
	// Set ammoCost and maxAmmo, maxAmmo below 0 means unlimited ammo
	void setAmmo(int cost, int maxAmmo);
	void setDamage(int damage);
	void setFireRate(double delay);
	void setBulletSpread(double angle);
	void setKnockBack(const sf::Vector2f &knockBack);
	void setBulletSpeed(double speed);
	void setWeaponSheet(const sf::Texture &weaponSheet, const sf::Texture &bulletSheet);
	// Bullet animation and bullet fire animation on this animation
	void setBulletAnimation(const jl::FrameAnimation &animation);

	void fire();

	void updateBullets(double deltaTime);
	void renderBullets(sf::RenderTarget &target);
	void render(sf::RenderTarget &target);

	std::string getStance() const;
	std::string getName() const;
	int getLevel() const;
	int getAmmo() const;
	int getMaxAmmo() const;

	bool hasUnlimitedAmmo() const;
	bool hasFullAmmo() const;

	std::string toAmmoString();
};

#endif