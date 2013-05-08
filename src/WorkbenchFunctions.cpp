#include "WorkbenchFunctions.h"
#include "Weapon.h"
#include "Player.h"
#include "MessageLog.h"

namespace WorkbenchFunctions
{
	int defaultCostCalc(int upgradeLevel, int cost)
	{
		return cost*upgradeLevel;
	};
	int ammoCostCalc(int upgradeLevel, int cost)
	{
		return cost;
	};

	void defaultPurchase(WorkbenchItem &item, Player *player)
	{
		player->removeCurrency(item.weaponCostCalc(item.weapon->getLevel(), item.cost));

		// First time purchase
		if(!item.isBought)
		{
			player->addWeapon(item.weapon);
			MessageLog::addMessage("You bought a " + item.weapon->getName());
		}

		// Weapon upgrade
		else
		{
			item.weapon->upgrade();
			MessageLog::addMessage("You upgraded your " + item.weapon->getName());
		}

	};
	void ammoPurchase(WorkbenchItem &item, Player *player)
	{
		int cost = item.weaponCostCalc(item.upgradeLevel, item.cost);
		if(player->getAmmo() + cost*2 <= TileCharacter::maxAmmo)
		{
			player->addAmmo(cost*2);
			player->removeCurrency(cost*2);
		}
	};
	void healthPurchase(WorkbenchItem &item, Player *player)
	{
		player->removeCurrency(item.weaponCostCalc(item.upgradeLevel, item.cost));

		if(item.upgradeLevel < item.maxUpgradeLevel)
		{
			item.upgradeLevel += 1;

			player->setMaxHealth(player->getMaxHealth()+1);
			MessageLog::addMessage("Health pack increased max life by 1");
		}
	};
	void speedPurchase(WorkbenchItem &item, Player *player)
	{
		player->removeCurrency(item.weaponCostCalc(item.upgradeLevel, item.cost));

		if(item.upgradeLevel < item.maxUpgradeLevel)
		{
			item.upgradeLevel += 1;

			player->setSpeed(player->getSpeed()+10);
			MessageLog::addMessage("Speed boost increased movement speed by 10");
		}
	};	
};