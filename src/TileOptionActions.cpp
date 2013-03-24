#include "TileOptionActions.h"
#include "TileMap.h"
#include "MessageLog.h"
#include "TileOptionManager.h"

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
		if(tileMap->getTile(tileIndex).getHealth() == tileMap->getTile(tileIndex).getMaxHealth())
		{
			MessageLog::addMessage("It's already fully repaired");
			return;
		}

		if(TileOptionManager::getPlayer()->pay(repairCost))
		{
			tileMap->getTile(tileIndex).repair(1);
			MessageLog::addMessage("*Spent 5 materials on repairing*");
			return;
		}
		else
			MessageLog::addMessage("I don't have enough material to repair");
	}
	void build(TileMap *tileMap, const sf::Vector2i &tileIndex)
	{
		if(TileOptionManager::getPlayer()->pay(repairCost*2))
		{
			MessageLog::addMessage("*Spent 5 materials on building a Window*");
			tileMap->changeTile(5, tileIndex.x, tileIndex.y, true);
			return;
		}
		else
			MessageLog::addMessage("I don't have enough material to build a window");
	}


	// TODO: TESTING
	void damageDoor(TileMap *tileMap, const sf::Vector2i &tileIndex)
	{
		tileMap->getTile(tileIndex).damage(1);
	}
};