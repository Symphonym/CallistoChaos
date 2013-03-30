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
	if(tilecharacter.getDirection() == TileCharacter::WalkingRight)
		tilecharacter.setDirection(TileCharacter::LookingRight);
	else if(tilecharacter.getDirection() == TileCharacter::WalkingLeft)
		tilecharacter.setDirection(TileCharacter::LookingLeft);
	else if(tilecharacter.getDirection() == TileCharacter::WalkingUp)
		tilecharacter.setDirection(TileCharacter::LookingUp);
	else if(tilecharacter.getDirection() == TileCharacter::WalkingDown)
		tilecharacter.setDirection(TileCharacter::LookingDown);
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
	for(auto it = m_characters.begin(); it != m_characters.end(); it++)
		it->second->events(events);
}
void CharacterManager::update(double deltaTime)
{
	for(auto it = m_characters.begin(); it != m_characters.end(); it++)
	{
		// Check if character is dead, if so, delete it
		if(it->second->isDead())
		{
			std::size_t index = it->first;
			m_characters.erase(it);

			// Move the previous character to this spot
			if(!m_characters.empty())
			{
				--m_charId;

				// Move previous character to temp variable, and delete previous element
				std::unique_ptr<TileCharacter> previousChar(std::move(m_characters[m_charId]));
				m_characters.erase(m_characters.find(m_charId));

				m_characters.insert(std::make_pair(index, std::move(previousChar)));
			}

			continue;
		}

		// Update walking of character
		if(it->second->isWalking())
		{
			if(it->second->getDirection() == TileCharacter::WalkingRight)
			{
				it->second->getSprite().move(it->second->getSpeed()*deltaTime, 0);
				it->second->characterEvents(TileCharacter::WalkingRight);
			}
			else if(it->second->getDirection() == TileCharacter::WalkingLeft)
			{
				it->second->getSprite().move(-it->second->getSpeed()*deltaTime, 0);
				it->second->characterEvents(TileCharacter::WalkingLeft);
			}
			else if(it->second->getDirection() == TileCharacter::WalkingUp)
			{	
				it->second->getSprite().move(0, -it->second->getSpeed()*deltaTime);
				it->second->characterEvents(TileCharacter::WalkingUp);
			}
			else if(it->second->getDirection() == TileCharacter::WalkingDown)
			{
				it->second->getSprite().move(0, it->second->getSpeed()*deltaTime);
				it->second->characterEvents(TileCharacter::WalkingDown);
			}

			// Position of the tile that the Character is walking towards (Tile index modified in character class)
			sf::Vector2f targetPosition = m_tileMap->getTilePosition(it->second->getIndex().x, it->second->getIndex().y);
			// Size of the tiles
			std::size_t tileSize = m_tileMap->getTileSize();
			// Character data
			sf::FloatRect characterBounds = it->second->getSprite().getGlobalBounds();
			TileCharacter::Event characterDirection = it->second->getDirection();

			// Check if the walking Character has reached it's target
			if(((characterBounds.left + characterBounds.width) >= (targetPosition.x + tileSize) && characterDirection == TileCharacter::WalkingRight) || // RIGHT
				(characterBounds.left <= targetPosition.x && characterDirection == TileCharacter::WalkingLeft) || // LEFT
				(characterBounds.top <= targetPosition.y && characterDirection == TileCharacter::WalkingUp) || // UP
				((characterBounds.top + characterBounds.height) >= (targetPosition.y + tileSize) && characterDirection == TileCharacter::WalkingDown)) // DOWN
					stopCharacter(*it->second, targetPosition);
		}

		// Update idle actions
		else if(!it->second->isWalking())
		{
			if(it->second->getDirection() == TileCharacter::LookingRight)
				it->second->characterEvents(TileCharacter::LookingRight);
			else if(it->second->getDirection() == TileCharacter::LookingLeft)
				it->second->characterEvents(TileCharacter::LookingLeft);
			else if(it->second->getDirection() == TileCharacter::LookingUp)
				it->second->characterEvents(TileCharacter::LookingUp);
			else if(it->second->getDirection() == TileCharacter::LookingDown)
				it->second->characterEvents(TileCharacter::LookingDown);
		}

		it->second->update(deltaTime);
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