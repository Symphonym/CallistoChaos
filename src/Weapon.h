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
		sf::Vector2f position, bulletPosition;
		double rotation;
		sf::IntRect subRect;
	};
	struct BulletData
	{
		sf::Sprite sprite;
		BulletDirections direction;
		jl::FrameAnimation animation;
	};

private:
	
	// Name of the weapon
	std::string m_name;

	// Bullets
	std::vector<BulletData> m_bullets;
	jl::FrameAnimation m_bulletAnimation;

	// Different weapon stances
	std::map<std::string, StanceData> m_stances;
	std::string m_activeStance;

	// Character to shoot from
	TileCharacter *m_trackedCharacter;

	sf::Sprite m_weaponSprite;
	const sf::Texture *m_bulletSheet;


	int m_ammoCost, m_ammo, m_maxAmmo, m_damage, m_upgradeLevel;
	double m_fireRate;
	double m_bulletSpeed;
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
	void addStance(const std::string &name, const sf::Vector2f &pos, const sf::IntRect &subRect, const sf::Vector2f &bulletPos = sf::Vector2f(0,0), double rotation = 0);
	void upgrade();
	// Puts ammo into weapon, returns leftovers
	int reload(int ammo);

	void setStance(const std::string &name);
	// Set ammoCost and maxAmmo, maxAmmo below 0 means unlimited ammo
	void setAmmo(int cost, int maxAmmo);
	void setDamage(int damage);
	void setFireRate(double delay);
	void setBulletSpeed(double speed);
	void setWeaponSheet(const sf::Texture &weaponSheet, const sf::Texture &bulletSheet);
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