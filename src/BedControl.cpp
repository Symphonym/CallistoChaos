#include "BedControl.h"
#include "Player.h"
#include "TileMap.h"
#include "MessageLog.h"

BedControl::BedControl() :
	m_player(nullptr),
	m_inBed(false),
	m_regenDelay(1)
{

}
BedControl::BedControl(Player *player) :
	m_player(player),
	m_inBed(false),
	m_regenDelay(1)
{

}

void BedControl::update()
{
	if(m_inBed)
	{
		if(m_regenClock.getElapsedTime().asSeconds() >= m_regenDelay)
		{
			m_regenClock.restart();
			m_player->heal(1);

			if(!m_player->isFullHealth())
				MessageLog::addMessage("1 health regenerated from sleep.");
		}
		else if(m_player->isFullHealth())
			MessageLog::addSingleMessage("You are fully rested");
	}
}

void BedControl::setRegenDelay(double secondDelay)
{
	m_regenDelay = secondDelay;
}

void BedControl::toggleBed(const sf::Vector2i &tileIndex)
{

	TileMap *map = &m_player->getTileMap();
	if(!m_inBed)
	{
		m_inBed = true;
		m_regenClock.restart();
		m_player->getSprite().setPosition(map->getTilePosition(tileIndex.x, tileIndex.y));
	}
	else if(m_inBed)
	{
		m_player->getSprite().setPosition(map->getTilePosition(m_player->getIndex().x, m_player->getIndex().y));
		m_inBed = false;
	}
}
bool BedControl::isInUse() const
{
	return m_inBed;
}