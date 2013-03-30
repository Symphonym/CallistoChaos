#include "WorkbenchFunctions.h"
#include "Weapon.h"
#include "Player.h"
#include "MessageLog.h"

namespace WorkbenchFunctions
{
	int defaultCostCalc(Weapon *weapon, int cost)
	{
		return cost*weapon->getLevel();
	};
	int ammoCostCalc(Weapon *weapon, int cost)
	{
		return cost;
	};

	void defaultPurchase(std::shared_ptr<Weapon> weapon, Player *player, int cost, bool firstTimePurchase, int maxUpgradeLevel)
	{
		player->removeCurrency(cost);

		// First time purchase, maxUpgradeLevel < 0 symbols this
		if(firstTimePurchase)
		{
			player->addWeapon(weapon);
			MessageLog::addMessage("You bought a " + weapon->getName());
		}

		// Weapon upgrade
		else
		{
			weapon->upgrade();
			MessageLog::addMessage("You upgraded your " + weapon->getName());
		}

	};
	void ammoPurchase(std::shared_ptr<Weapon> weapon, Player *player, int cost, bool firstTimePurchase, int maxUpgradeLevel)
	{
		if(player->getAmmo() + 5 <= TileCharacter::maxAmmo)
		{
			player->addAmmo(5);
			player->removeCurrency(5);
		}
	};
};