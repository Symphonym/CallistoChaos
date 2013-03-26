#include "BedControl.h"
#include "Player.h"
#include "TileMap.h"

BedControl::BedControl() :
	m_player(nullptr),
	m_inBed(false)
{

}

void BedControl::providePlayer(Player *player)
{
	m_player = player;
}
void BedControl::toggleBed(const sf::Vector2i &tileIndex)
{

	TileMap *map = &m_player->getTileMap();
	if(!m_inBed)
	{
		m_inBed = true;

		m_player->getSprite().setPosition(map->getTilePosition(tileIndex.x, tileIndex.y));
	}
	else if(m_inBed)
	{
		m_player->getSprite().setPosition(map->getTilePosition(m_player->getIndex().x, m_player->getIndex().y));
		m_inBed = false;
		/*
		TileMap *map = &m_player->getTileMap();

		// Attempt to place player above bed
		if(tileIndex.y - 1 >= 0 && !map->getTile(tileIndex.x, tileIndex.y - 1).isSolid())
		{
			m_player->getSprite().setPosition(map->getTilePosition(tileIndex.x, tileIndex.y - 1));
			m_inBed = false;
		}
		// Attempt to place plaver below bed
		else if(tileIndex.y + 1 < map->getMapSize().y && !map->getTile(tileIndex.x, tileIndex.y + 1).isSolid())
		{
			m_player->getSprite().setPosition(map->getTilePosition(tileIndex.x, tileIndex.y + 1));
			m_inBed = false;
		}
		// Attempt to place player to the left of the bed
		else if(tileIndex.x - 1 >= 0 && !map->getTile(tileIndex.x - 1, tileIndex.y).isSolid())
		{
			m_player->getSprite().setPosition(map->getTilePosition(tileIndex.x - 1, tileIndex.y));
			m_inBed = false;
		}
		// Attempt to place player to the right of the bed
		else if(tileIndex.x + 1 < map->getMapSize().x && !map->getTile(tileIndex.x + 1, tileIndex.y).isSolid())
		{
			m_player->getSprite().setPosition(map->getTilePosition(tileIndex.x + 1, tileIndex.y));
			m_inBed = false;
		}*/
	}
}

bool BedControl::isInUse() const
{
	return m_inBed;
}