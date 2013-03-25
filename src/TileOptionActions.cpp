#include "TileOptionActions.h"
#include "TileMap.h"
#include "MessageLog.h"
#include "TileOptionManager.h"
#include "Utility.h"

namespace TileOptionActions
{
	static const int repairCost = 5;

	void openDoor(TileMap *tileMap, const sf::Vector2i &tileIndex)
	{
		if(!tileMap->getTile(tileIndex).isDestroyed())
			tileMap->changeTile(7, tileIndex.x, tileIndex.y);
	}
	void closeDoor(TileMap *tileMap, const sf::Vector2i &tileIndex)
	{
		tileMap->changeTile(6, tileIndex.x, tileIndex.y);
	}

	void examineHealth(TileMap *tileMap, const sf::Vector2i &tileIndex)
	{
		Tile *tile = &tileMap->getTile(tileIndex);

		if(tile->isImmortal())
			MessageLog::addMessage("It can't possibly be destroyed");
		else
		{
			double healthRatio = 
			(double)tile->getHealth()/
			(double)tile->getMaxHealth();

			if(healthRatio <= 0.0)
				MessageLog::addMessage("There's nothing left, it's completely destroyed");
			else if(healthRatio <= 0.1)
				MessageLog::addMessage("It's on the brink of destruction");
			else if(healthRatio <= 0.3)
				MessageLog::addMessage("It can only take a few more bruises");
			else if(healthRatio <= 0.5)
				MessageLog::addMessage("It can probably take as many hits more");
			else if(healthRatio <= 0.7)
				MessageLog::addMessage("There's just a few scratches here and there");
			else
				MessageLog::addMessage("Doesn't look like anything has touched it");
		}
	}
	void repair(TileMap *tileMap, const sf::Vector2i &tileIndex)
	{
		const int repairCost = 5;

		if(tileMap->getTile(tileIndex).getHealth() == tileMap->getTile(tileIndex).getMaxHealth())
		{
			MessageLog::addMessage("It's already fully repaired");
			return;
		}

		else if(TileOptionManager::getPlayer()->affordCurrency(repairCost))
		{
			TileOptionManager::getPlayer()->removeCurrency(repairCost);
			tileMap->getTile(tileIndex).repair(1);
			MessageLog::addMessage("*Spent " + jl::Util::toString(repairCost) +  " materials on repairing*");
			return;
		}
		else
			MessageLog::addMessage("I don't have enough material to repair");
	}
	void build(TileMap *tileMap, const sf::Vector2i &tileIndex)
	{
		const int buildCost = 10;

		if(TileOptionManager::getPlayer()->affordCurrency(buildCost))
		{
			TileOptionManager::getPlayer()->removeCurrency(buildCost);
			MessageLog::addMessage("*Spent " + jl::Util::toString(buildCost) + " materials on building a Window*");
			tileMap->changeTile(5, tileIndex.x, tileIndex.y, true);
			return;
		}
		else
			MessageLog::addMessage("I don't have enough material to build a window");
	}

	void reload(TileMap *tileMap, const sf::Vector2i &tileIndex)
	{
		const int reloadAmount = 10;

		Player *player = TileOptionManager::getPlayer();

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
};