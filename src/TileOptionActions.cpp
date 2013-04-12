#include "TileOptionActions.h"
#include "TileMap.h"
#include "MessageLog.h"
#include "TileOptionManager.h"
#include "Utility.h"
#include "GameState.h"
#include <iostream>
namespace TileOptionActions
{
	static const int repairCost = 5;

	void openDoor(TileMap *tileMap, const sf::Vector2i &tileIndex, TileOptionManager *manager)
	{
		if(!tileMap->getTile(tileIndex).isDestroyed())
			tileMap->changeTile(7, tileIndex.x, tileIndex.y);

	}
	void closeDoor(TileMap *tileMap, const sf::Vector2i &tileIndex, TileOptionManager *manager)
	{
		// Can't close door if someone stands on it
		if(!tileMap->getTile(tileIndex).isOccupied())
			tileMap->changeTile(6, tileIndex.x, tileIndex.y);
	}

	void examineHealth(TileMap *tileMap, const sf::Vector2i &tileIndex, TileOptionManager *manager)
	{
		Tile *tile = &tileMap->getTile(tileIndex);

		if(tile->isImmortal())
			MessageLog::addMessage("It can't possibly be destroyed (inf\%)");
		else
		{
			double healthRatio = 
				(double)tile->getHealth()/
				(double)tile->getMaxHealth();


			std::string hpLeftString = jl::Util::toString(healthRatio*100);

			if(healthRatio <= 0.0)
				MessageLog::addMessage("There's nothing left, it's completely destroyed (" + hpLeftString + "\%)");
			else if(healthRatio <= 0.1)
				MessageLog::addMessage("It's on the brink of destruction (" + hpLeftString + "\%)");
			else if(healthRatio <= 0.3)
				MessageLog::addMessage("It can only take a few more bruises (" + hpLeftString + "\%)");
			else if(healthRatio <= 0.5)
				MessageLog::addMessage("It can probably take as many hits more (" + hpLeftString + "\%)");
			else if(healthRatio <= 0.7)
				MessageLog::addMessage("There's just a few scratches here and there (" + hpLeftString + "\%)");
			else
				MessageLog::addMessage("Doesn't look like anything has touched it (" + hpLeftString + "\%)");
		}
	}
	void repair(TileMap *tileMap, const sf::Vector2i &tileIndex, TileOptionManager *manager)
	{
		const int repairCost = 1;

		if(tileMap->getTile(tileIndex).getHealth() == tileMap->getTile(tileIndex).getMaxHealth())
		{
			MessageLog::addMessage("It's already fully repaired");
			return;
		}

		else if(manager->getPlayer()->affordCurrency(repairCost))
		{
			manager->getPlayer()->removeCurrency(repairCost);
			tileMap->getTile(tileIndex).repair(1);
			MessageLog::addMessage("*Spent " + jl::Util::toString(repairCost) +  " materials on repairing*");
			return;
		}
		else
			MessageLog::addMessage("I need " + jl::Util::toString(repairCost - manager->getPlayer()->getCurrency()) + " more material to repair.");
	}
	void build(TileMap *tileMap, const sf::Vector2i &tileIndex, TileOptionManager *manager)
	{
		const int buildCost = 20;

		if(manager->getPlayer()->affordCurrency(buildCost))
		{
			manager->getPlayer()->removeCurrency(buildCost);
			MessageLog::addMessage("*Spent " + jl::Util::toString(buildCost) + " materials on building a Window*");
			tileMap->changeTile(5, tileIndex.x, tileIndex.y, true);
			return;
		}
		else
			MessageLog::addMessage("I need " + jl::Util::toString(buildCost - manager->getPlayer()->getCurrency()) + " more material to build a window.");
	}

	void reload(TileMap *tileMap, const sf::Vector2i &tileIndex, TileOptionManager *manager)
	{
		const int reloadAmount = 10;

		Player *player = manager->getPlayer();

		// Player has unlimited ammo
		if(player->getActiveWeapon()->hasUnlimitedAmmo())
		{
			MessageLog::addMessage("The " + player->getActiveWeapon()->getName() + " can't be reloaded");
			return;
		}

		// Player out of ammo to reload with
		if(player->getAmmo() == 0)
		{
			MessageLog::addMessage("There's no ammo to reload with");
			return;
		}

		// Player has full ammo already
		if(player->getActiveWeapon()->hasFullAmmo())
		{
			MessageLog::addMessage("The weapon is already fully loaded");
			return;
		}

		if(player->affordAmmo(reloadAmount))
		{
			player->removeAmmo(reloadAmount);

			// Give back leftovers to player
			int leftOvers = player->getActiveWeapon()->reload(reloadAmount);
			player->addAmmo(leftOvers);

			MessageLog::addMessage("Reloaded " + player->getActiveWeapon()->getName() +
				" with " + jl::Util::toString(reloadAmount - leftOvers) + " bullets");
		}
		else if(player->getAmmo() < reloadAmount)
		{
			// Give back leftovers to player
			player->addAmmo(player->getActiveWeapon()->reload(player->getAmmo()));

			MessageLog::addMessage("Reloaded " + player->getActiveWeapon()->getName() +
				" with " + jl::Util::toString(player->getAmmo()) + " bullets");

			player->removeAmmo(player->getAmmo());

		}
	}
	void sleep(TileMap *tileMap, const sf::Vector2i &tileIndex, TileOptionManager *manager)
	{
		manager->getPlayer()->getGame().getBed().toggleBed(tileIndex);

		// Remove sleep message
		if(manager->getPlayer()->getGame().getBed().isInUse())
		{
			manager->removeOption(10, 0);
			manager->addOption(10, "Wake up", TileOptionActions::sleep);
			MessageLog::addMessage("You are now resting");
		}
		else
		{
			manager->removeOption(10, 0);
			manager->addOption(10, "Sleep", TileOptionActions::sleep);
			MessageLog::addMessage("You woke up from your sleep");
		}
	}
	void craft(TileMap *tileMap, const sf::Vector2i &tileIndex, TileOptionManager *manager)
	{
		manager->getPlayer()->setBusy(true);
		manager->getPlayer()->getGame().getWorkbench().setVisible(true);
	} 
};