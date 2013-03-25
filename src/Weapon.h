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
protected:

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

	std::vector<BulletData> m_bullets;

	// Character to shoot from
	TileCharacter *m_trackedCharacter;
	// Sprite for weapon
	sf::Sprite m_weaponSprite;

	// Pointer to bullet sheet texture
	const sf::Texture *m_bulletSheet;

	// Different weapon stances
	std::map<std::string, StanceData> m_stances;
	std::string m_activeStance;

	int m_ammoCost;
	double m_fireRate;
	double m_bulletSpeed;
	sf::Clock m_fireRateClock;
	// Name of the weapon
	std::string m_name;
	// Bullet animation data
	jl::FrameAnimation m_bulletAnimation;

	sf::Vector2i getBulletIndex(const BulletData &bullet) const;
	sf::Vector2f getWeaponPos();

	virtual void update(std::size_t index, double deltaTime){};
	virtual void render(std::size_t index, sf::RenderTarget &target){};

public:

	explicit Weapon(const std::string &name, TileCharacter *tileCharacter, jl::AssetManager &assets);

	// Add a stance, position is relative to the tracked targe
	void addStance(const std::string &name, const sf::Vector2f &pos, const sf::IntRect &subRect, const sf::Vector2f &bulletPos = sf::Vector2f(0,0), double rotation = 0);

	void setStance(const std::string &name);
	void setAmmoCost(int cost);
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
};

#endif