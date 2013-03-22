#include "CharacterManager.h"
#include "TileMap.h"


CharacterManager::CharacterManager() :
	m_tileMap(nullptr)
{

}

void CharacterManager::stopCharacter(TileCharacter &tilecharacter, const sf::Vector2f &tilePosition)
{
	tilecharacter.getSprite().setPosition(tilePosition.x, tilePosition.y);
	tilecharacter.setWalking(false);


	if(tilecharacter.getDirection() == TileCharacter::Right)
		tilecharacter.setDirection(TileCharacter::IdleRight);
	else if(tilecharacter.getDirection() == TileCharacter::Left)
		tilecharacter.setDirection(TileCharacter::IdleLeft);
	else if(tilecharacter.getDirection() == TileCharacter::Up)
		tilecharacter.setDirection(TileCharacter::IdleUp);
	else if(tilecharacter.getDirection() == TileCharacter::Down)
		tilecharacter.setDirection(TileCharacter::IdleDown);
}

void CharacterManager::registerTileMap(TileMap &tilemap)
{
	m_tileMap = &tilemap;
}

void CharacterManager::addCharacter(std::unique_ptr<TileCharacter> tilecharacter)
{
	m_characters.push_back(std::move(tilecharacter));
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
		// Update walking of character
		if(m_characters[i]->isWalking())
		{
			if(m_characters[i]->getDirection() == TileCharacter::Right)
				m_characters[i]->getSprite().move(m_characters[i]->getSpeed()*deltaTime, 0);
			else if(m_characters[i]->getDirection() == TileCharacter::Left)
				m_characters[i]->getSprite().move(-m_characters[i]->getSpeed()*deltaTime, 0);
			else if(m_characters[i]->getDirection() == TileCharacter::Up)
				m_characters[i]->getSprite().move(0, -m_characters[i]->getSpeed()*deltaTime);
			else if(m_characters[i]->getDirection() == TileCharacter::Down)
				m_characters[i]->getSprite().move(0, m_characters[i]->getSpeed()*deltaTime);

			// Position of the tile that the Character is walking towards
			sf::Vector2f targetPosition = m_tileMap->getTilePosition(m_characters[i]->getIndex().x, m_characters[i]->getIndex().y);
			// Size of the tiles
			std::size_t tileSize = m_tileMap->getTileSize();
			// Character data
			sf::FloatRect characterBounds = m_characters[i]->getSprite().getGlobalBounds();
			TileCharacter::Directions characterDirection = m_characters[i]->getDirection();

			// Check if the walking Character has reached it's target
			if(((characterBounds.left + characterBounds.width) >= (targetPosition.x + tileSize) && characterDirection == TileCharacter::Right) || // RIGHT
				(characterBounds.left <= targetPosition.x && characterDirection == TileCharacter::Left) || // LEFT
				(characterBounds.top <= targetPosition.y && characterDirection == TileCharacter::Up) || // UP
				((characterBounds.top + characterBounds.height) >= (targetPosition.y + tileSize) && characterDirection == TileCharacter::Down)) // DOWN
					stopCharacter(*m_characters[i], targetPosition);
		}
		m_characters[i]->update(deltaTime);
	}
}
void CharacterManager::render(sf::RenderTarget &target)
{
	for(std::size_t i = 0; i < m_characters.size(); i++)
		m_characters[i]->render(target);
}

TileCharacter &CharacterManager::getCharacter(int index)
{
	return *m_characters[index];
}