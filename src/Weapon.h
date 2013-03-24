#ifndef WEAPON_H
#define WEAPON_H

#include <SFML/Graphics.hpp>
#include <map>
#include "AssetManager.h"
#include "TileCharacter.h"

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
		sf::Vector2f position;
		double rotation;
		sf::IntRect subRect;
	};

	std::vector<std::pair<sf::Vector2f, BulletDirections>> m_bullets;

	TileCharacter *m_trackedCharacter;
	sf::Sprite m_weaponSprite, m_bulletSprite;

	std::map<std::string, StanceData> m_stances;
	std::string m_activeStance;

	int m_ammoCost;
	double m_fireRate;
	double m_bulletSpeed;
	sf::Clock m_fireRateClock;

public:

	explicit Weapon(TileCharacter *tileCharacter, jl::AssetManager &assets);

	// Add a stance, position is relative to the tracked targe
	void addStance(const std::string &name, const sf::Vector2f &pos, const sf::IntRect &subRect, double rotation = 0);

	void setStance(const std::string &name);
	void setAmmoCost(int cost);
	void setFireRate(double delay);
	void setBulletSpeed(double speed);
	void setWeaponSheet(const sf::Texture &weaponSheet, const sf::Texture &bulletSheet);

	void fire();

	virtual void update(double deltaTime){};
	virtual void render(sf::RenderTarget &target){};

	std::string getStance() const;
};

#endif