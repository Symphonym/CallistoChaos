#include "CharacterManager.h"
#include "TileMap.h"


CharacterManager::CharacterManager() :
	m_tileMap(nullptr),
	m_charId(0)
{

}

void CharacterManager::stopCharacter(TileCharacter &tilecharacter, const sf::Vector2f &tilePosition)
{
	// Set character position exactly at tile and set Walking to false
	tilecharacter.getSprite().setPosition(tilePosition.x, tilePosition.y);
	tilecharacter.setWalking(false);

	// Switch to corresponding idle state
	if(tilecharacter.getDirection() == TileCharacter::WalkRight)
		tilecharacter.setDirection(TileCharacter::IdleRight);
	else if(tilecharacter.getDirection() == TileCharacter::WalkLeft)
		tilecharacter.setDirection(TileCharacter::IdleLeft);
	else if(tilecharacter.getDirection() == TileCharacter::WalkUp)
		tilecharacter.setDirection(TileCharacter::IdleUp);
	else if(tilecharacter.getDirection() == TileCharacter::WalkDown)
		tilecharacter.setDirection(TileCharacter::IdleDown);
}

void CharacterManager::registerTileMap(TileMap &tilemap)
{
	m_tileMap = &tilemap;
}

void CharacterManager::addCharacter(std::unique_ptr<TileCharacter> tilecharacter)
{
	m_characters[m_charId] = std::move(tilecharacter);
	m_tileMap->getTile(m_characters[m_charId]->getIndex().x, m_characters[m_charId]->getIndex().y).
		setCharacter(m_characters[m_charId].get());

	++m_charId;
}

void CharacterManager::events(sf::Event &events)
{
	for(std::size_t i = 0; i < m_characters.size(); i++)
		m_characters[i]->events(events);
}
void CharacterManager::update(double deltaTime)
{
	for(std::size_t i = 0; i < m_characters.size(); i++)
	{
		// Check if character is dead, if so, delete it
		if(m_characters[i]->isDead())
		{
			m_characters.erase(m_characters.find(i));

			// Move the previous character to this spot
			if(!m_characters.empty())
			{
				--m_charId;

				// Move previous character to temp variable, and delete previous element
				std::unique_ptr<TileCharacter> previousChar(std::move(m_characters[m_charId]));
				m_characters.erase(m_characters.find(m_charId));

				m_characters.insert(std::make_pair(i, std::move(previousChar)));
			}

			continue;
		}

		// Update walking of character
		if(m_characters[i]->isWalking())
		{
			if(m_characters[i]->getDirection() == TileCharacter::WalkRight)
			{
				m_characters[i]->getSprite().move(m_characters[i]->getSpeed()*deltaTime, 0);
				m_characters[i]->duringWalkRight();
			}
			else if(m_characters[i]->getDirection() == TileCharacter::WalkLeft)
			{
				m_characters[i]->getSprite().move(-m_characters[i]->getSpeed()*deltaTime, 0);
				m_characters[i]->duringWalkLeft();
			}
			else if(m_characters[i]->getDirection() == TileCharacter::WalkUp)
			{	
				m_characters[i]->getSprite().move(0, -m_characters[i]->getSpeed()*deltaTime);
				m_characters[i]->duringWalkUp();
			}
			else if(m_characters[i]->getDirection() == TileCharacter::WalkDown)
			{
				m_characters[i]->getSprite().move(0, m_characters[i]->getSpeed()*deltaTime);
				m_characters[i]->duringWalkDown();
			}

			// Position of the tile that the Character is walking towards
			sf::Vector2f targetPosition = m_tileMap->getTilePosition(m_characters[i]->getIndex().x, m_characters[i]->getIndex().y);
			// Size of the tiles
			std::size_t tileSize = m_tileMap->getTileSize();
			// Character data
			sf::FloatRect characterBounds = m_characters[i]->getSprite().getGlobalBounds();
			TileCharacter::Directions characterDirection = m_characters[i]->getDirection();

			// Check if the walking Character has reached it's target
			if(((characterBounds.left + characterBounds.width) >= (targetPosition.x + tileSize) && characterDirection == TileCharacter::WalkRight) || // RIGHT
				(characterBounds.left <= targetPosition.x && characterDirection == TileCharacter::WalkLeft) || // LEFT
				(characterBounds.top <= targetPosition.y && characterDirection == TileCharacter::WalkUp) || // UP
				((characterBounds.top + characterBounds.height) >= (targetPosition.y + tileSize) && characterDirection == TileCharacter::WalkDown)) // DOWN
					stopCharacter(*m_characters[i], targetPosition);
		}

		// Update idle actions
		else if(!m_characters[i]->isWalking())
		{
			if(m_characters[i]->getDirection() == TileCharacter::IdleRight)
				m_characters[i]->duringIdleRight();
			else if(m_characters[i]->getDirection() == TileCharacter::IdleLeft)
				m_characters[i]->duringIdleLeft();
			else if(m_characters[i]->getDirection() == TileCharacter::IdleUp)
				m_characters[i]->duringIdleUp();
			else if(m_characters[i]->getDirection() == TileCharacter::IdleDown)
				m_characters[i]->duringIdleDown();
		}

		m_characters[i]->update(deltaTime);
	}
}
void CharacterManager::render(sf::RenderTarget &target)
{
	for(auto it = m_characters.begin(); it != m_characters.end(); it++)
		it->second->render(target);
}

TileCharacter &CharacterManager::getCharacter(int index)
{
	return *m_characters[index];
}