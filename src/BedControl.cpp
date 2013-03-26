#include "BedControl.h"
#include "Player.h"
#include "TileMap.h"

BedControl::BedControl() :
	m_player(nullptr),
	m_tileIndex(0,0),
	m_inBed(false)
{

}

void BedControl::providePlayer(Player *player)
{
	m_player = player;
}
void BedControl::toggleBed(const sf::Vector2i &tileIndex)
{
	if(!m_inBed)
	{
		m_tileIndex = tileIndex;
		m_inBed = true;

		TileMap *map = &m_player->getTileMap();
		m_player->getSprite().setPosition(map->getTilePosition(tileIndex.x, tileIndex.y));
	}
	else if(m_inBed)
	{
		TileMap *map = &m_player->getTileMap();

		// Attempt to place player above bed
		if(m_tileIndex.y - 1 >= 0 && !map->getTile(m_tileIndex.x, m_tileIndex.y - 1).isSolid())
		{
			m_player->getSprite().setPosition(map->getTilePosition(m_tileIndex.x, m_tileIndex.y - 1));
			m_inBed = false;
		}
		// Attempt to place plaver below bed
		else if(m_tileIndex.y + 1 < map->getMapSize().y && !map->getTile(m_tileIndex.x, m_tileIndex.y + 1).isSolid())
		{
			m_player->getSprite().setPosition(map->getTilePosition(m_tileIndex.x, m_tileIndex.y + 1));
			m_inBed = false;
		}
		// Attempt to place player to the left of the bed
		else if(m_tileIndex.x - 1 >= 0 && !map->getTile(m_tileIndex.x - 1, m_tileIndex.y).isSolid())
		{
			m_player->getSprite().setPosition(map->getTilePosition(m_tileIndex.x - 1, m_tileIndex.y));
			m_inBed = false;
		}
		// Attempt to place player to the right of the bed
		else if(m_tileIndex.x + 1 < map->getMapSize().x && !map->getTile(m_tileIndex.x + 1, m_tileIndex.y).isSolid())
		{
			m_player->getSprite().setPosition(map->getTilePosition(m_tileIndex.x + 1, m_tileIndex.y));
			m_inBed = false;
		}
	}
}

bool BedControl::isInUse() const
{
	return m_inBed;
}