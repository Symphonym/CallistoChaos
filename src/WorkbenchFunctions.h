#ifndef WORKBENCHFUNCTIONS_H
#define WORKBENCHFUNCTIONS_H

#include <memory>

class Weapon;
class Player;
namespace WorkbenchFunctions
{
	int defaultCostCalc(Weapon *weapon, int cost);
	int ammoCostCalc(Weapon *weapon, int cost);

	void defaultPurchase(std::shared_ptr<Weapon> weapon, Player *player, int cost, bool firstTimePurchase, int maxUpgradeLevel);
	void ammoPurchase(std::shared_ptr<Weapon> weapon, Player *player, int cost, bool firstTimePurchase, int maxUpgradeLevel);
};

#endif