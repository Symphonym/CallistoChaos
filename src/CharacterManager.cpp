#include "CharacterManager.h"
#include "TileMap.h"
#include "GameState.h"

CharacterManager::CharacterManager() :
	m_tileMap(nullptr)
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

void CharacterManager::addCharacter(std::unique_ptr<TileCharacter> tilecharacter)
{
	m_characters.push_back(std::move(tilecharacter));
	m_tileMap->getTile(m_characters.back()->getIndex()).setCharacter(m_characters.back().get());
}
void CharacterManager::addPlayer(std::unique_ptr<Player> player)
{
	m_player = player.get();
	m_tileMap = &player.get()->getTileMap();
	addCharacter(std::move(player));
}

void CharacterManager::events(sf::Event &events)
{
	for(int i = 0; i < m_characters.size(); i++)
		m_characters[i]->events(events);
}

void CharacterManager::update(double deltaTime)
{
	for(int i = 0; i < m_characters.size(); i++)
	{
		// Check if character is dead, if so, delete it
		if(m_characters[i]->isDead() && !dynamic_cast<Player*>(m_characters[i].get()))
		{
			m_tileMap->getTile(m_characters[i]->getIndex()).clearCharacter();

			for(int e = 0; e < m_characters[i]->getCurrency(); e++)
				m_player->getGame().getLoot().spawnEntity(sf::Vector2f(
					m_characters[i]->getSprite().getPosition().x + m_characters[i]->getSprite().getGlobalBounds().width/2,
					m_characters[i]->getSprite().getPosition().y + m_characters[i]->getSprite().getGlobalBounds().height/2));

			m_characters.erase(m_characters.begin() + i);
			m_player->addScore(1);
			continue;
		}

		// Update walking of character
		if(m_characters[i]->isWalking())
		{
			if(m_characters[i]->getDirection() == TileCharacter::WalkingRight)
			{
				m_characters[i]->getSprite().move(m_characters[i]->getSpeed()*deltaTime, 0);
				m_characters[i]->characterEvents(TileCharacter::WalkingRight);
			}
			else if(m_characters[i]->getDirection() == TileCharacter::WalkingLeft)
			{
				m_characters[i]->getSprite().move(-m_characters[i]->getSpeed()*deltaTime, 0);
				m_characters[i]->characterEvents(TileCharacter::WalkingLeft);
			}
			else if(m_characters[i]->getDirection() == TileCharacter::WalkingUp)
			{	
				m_characters[i]->getSprite().move(0, -m_characters[i]->getSpeed()*deltaTime);
				m_characters[i]->characterEvents(TileCharacter::WalkingUp);
			}
			else if(m_characters[i]->getDirection() == TileCharacter::WalkingDown)
			{
				m_characters[i]->getSprite().move(0, m_characters[i]->getSpeed()*deltaTime);
				m_characters[i]->characterEvents(TileCharacter::WalkingDown);
			}

			// Position of the tile that the Character is walking towards (Tile index modified in character class)
			sf::Vector2f targetPosition = m_tileMap->getTilePosition(m_characters[i]->getIndex().x, m_characters[i]->getIndex().y);
			// Size of the tiles
			std::size_t tileSize = m_tileMap->getTileSize();
			// Character data
			sf::FloatRect characterBounds = m_characters[i]->getSprite().getGlobalBounds();
			TileCharacter::Event characterDirection = m_characters[i]->getDirection();

			// Check if the walking Character has reached it's target
			if(((characterBounds.left + characterBounds.width) >= (targetPosition.x + tileSize) && characterDirection == TileCharacter::WalkingRight) || // RIGHT
				(characterBounds.left <= targetPosition.x && characterDirection == TileCharacter::WalkingLeft) || // LEFT
				(characterBounds.top <= targetPosition.y && characterDirection == TileCharacter::WalkingUp) || // UP
				((characterBounds.top + characterBounds.height) >= (targetPosition.y + tileSize) && characterDirection == TileCharacter::WalkingDown)) // DOWN
					stopCharacter(*m_characters[i], targetPosition);
		}

		// Update idle actions
		else if(!m_characters[i]->isWalking())
		{
			if(m_characters[i]->getDirection() == TileCharacter::LookingRight)
				m_characters[i]->characterEvents(TileCharacter::LookingRight);
			else if(m_characters[i]->getDirection() == TileCharacter::LookingLeft)
				m_characters[i]->characterEvents(TileCharacter::LookingLeft);
			else if(m_characters[i]->getDirection() == TileCharacter::LookingUp)
				m_characters[i]->characterEvents(TileCharacter::LookingUp);
			else if(m_characters[i]->getDirection() == TileCharacter::LookingDown)
				m_characters[i]->characterEvents(TileCharacter::LookingDown);
		}

		m_characters[i]->update(deltaTime);
	}
}
void CharacterManager::render(sf::RenderTarget &target)
{
	for(int i = 0; i < m_characters.size(); i++)
		m_characters[i]->render(target);
}

Player &CharacterManager::getPlayer()
{
	return *m_player;
}
TileCharacter &CharacterManager::getCharacter(int index)
{
	return *m_characters[index];
}
std::size_t CharacterManager::getCount() const
{
	return m_characters.size();
}