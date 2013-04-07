#ifndef WORKBENCHFUNCTIONS_H
#define WORKBENCHFUNCTIONS_H

#include <SFML/Graphics.hpp>
#include <memory>

class Weapon;
class Player;

typedef std::function<int (int upgradeLevel, int cost)> WeaponCostCalcPtr;

struct WorkbenchItem
{
	// Cost calculator
	WeaponCostCalcPtr weaponCostCalc;
	// Action to do when the weapon is purchased/upgraded
	std::function<void (WorkbenchItem &item, Player *player)> weaponAction;

	// The weapon
	std::shared_ptr<Weapon> weapon;

	std::string name;
	bool isBought;
	sf::IntRect displaySubRect;

	int maxUpgradeLevel, upgradeLevel, cost;
	sf::Vector2f position;
	sf::Vector2f targetPosition;
};

typedef std::function<void (WorkbenchItem &item, Player *player)> WeaponActionPtr;

namespace WorkbenchFunctions
{
	int defaultCostCalc(int upgradeLevel, int cost);
	int ammoCostCalc(int upgradeLevel, int cost);

	void defaultPurchase(WorkbenchItem &item, Player *player);
	void ammoPurchase(WorkbenchItem &item, Player *player);
	void healthPurchase(WorkbenchItem &item, Player *player);
};

#endif