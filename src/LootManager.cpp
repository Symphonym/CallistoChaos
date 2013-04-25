#include "LootManager.h"
#include "Utility.h"
#include "Player.h"
#include "TileMap.h"

LootManager::LootManager(jl::AssetManager &assets) : 
	m_player(nullptr)
{
	m_sprite.setTexture(assets.getTexture("res/tiles.png"));
	m_sprite.setTextureRect(sf::IntRect(48, 0, 8, 7));
	m_sprite.setScale(0.8, 0.8);
	m_sprite.setOrigin(
		m_sprite.getGlobalBounds().width/2,
		m_sprite.getGlobalBounds().height/2);
}

void LootManager::providePlayer(Player *player)
{
	m_player = player;
}

void LootManager::spawnEntity(const sf::Vector2f &position)
{
	LootEntity entity;
	entity.position = position;

	int directionAngle = std::rand() % 360;
	entity.direction = sf::Vector2f(std::sin(directionAngle), std::cos(directionAngle));
	entity.direction = jl::Vec::normalize(entity.direction);
	entity.slideSpeed = std::rand() % 50;
	entity.speed = 0;
	entity.sineWave = directionAngle;
	entity.alphaFade = 255;

	m_entities.push_back(entity);
}

void LootManager::update(double deltaTime)
{
	for(int i = 0; i < m_entities.size(); i++)
	{
		// Initial sliding
		sf::Vector2f speedAdd(
			m_entities[i].direction.x*m_entities[i].slideSpeed*deltaTime,
			m_entities[i].direction.y*m_entities[i].slideSpeed*deltaTime);

		m_entities[i].sineWave += 10*deltaTime;

		if(m_entities[i].slideSpeed < 0)
			m_entities[i].slideSpeed = 0;
		if(m_entities[i].sineWave > 360)
			m_entities[i].sineWave = 0;

		moveEntity(m_entities[i], speedAdd);
		m_entities[i].slideSpeed -= 40*deltaTime;

		sf::Vector2f centerPlayer(
			m_player->getSprite().getPosition().x+m_player->getSprite().getGlobalBounds().width/2,
			m_player->getSprite().getPosition().y+m_player->getSprite().getGlobalBounds().height/2);

		float exactDistance = jl::Vec::length(centerPlayer - m_entities[i].position);

		// Drag entity towards player, if he's got space
		if(exactDistance < 60 && m_player->getCurrency() + 3 <= TileCharacter::maxCurrency)
		{
			m_entities[i].speed = 60.0 - exactDistance;
			sf::Vector2f normalizedDirection(jl::Vec::normalize(centerPlayer - m_entities[i].position));
			sf::Vector2f speedOffset(m_entities[i].speed*normalizedDirection.x*deltaTime, m_entities[i].speed*normalizedDirection.y*deltaTime);
			moveEntity(m_entities[i], speedOffset);

			if(exactDistance < 1.0)
			{
				// Each entity "contains" 3 currency
				m_player->addCurrency(3);
				m_entities.erase(m_entities.begin() + i);
				continue;
			}
		}

		// Fade entity 
		m_entities[i].alphaFade -= 25.0*deltaTime;

		if(m_entities[i].alphaFade <= 0)
		{
			m_entities.erase(m_entities.begin() + i);
			continue;
		}
	}
}
void LootManager::render(sf::RenderTarget &target)
{
	for(int i = 0; i < m_entities.size(); i++)
	{
		// Clamp entity to screen
		if(m_entities[i].position.x < 0)
			m_entities[i].position.x = 0;
		if(m_entities[i].position.y < 0)
			m_entities[i].position.y = 0;
		if(m_entities[i].position.x + m_sprite.getGlobalBounds().width > target.getView().getSize().x)
			m_entities[i].position.x = target.getView().getSize().x - m_sprite.getGlobalBounds().width;
		if(m_entities[i].position.y + m_sprite.getGlobalBounds().height > target.getView().getSize().y)
			m_entities[i].position.y = target.getView().getSize().y - m_sprite.getGlobalBounds().height;

		sf::Vector2f position(m_entities[i].position);
		position.y += std::sin(m_entities[i].sineWave);

		sf::Color fadeColor = sf::Color::White;
		fadeColor.a = m_entities[i].alphaFade;
		m_sprite.setColor(fadeColor);

		m_sprite.setPosition(position);
		target.draw(m_sprite);
	}
}

void LootManager::moveEntity(LootEntity &entity, const sf::Vector2f &speedOffset)
{

	sf::Vector2i tileIndex(
		(entity.position.x+speedOffset.x) / m_player->getTileMap().getTileSize(), 
		entity.position.y / m_player->getTileMap().getTileSize());

	tileIndex.x = jl::Math::clamp(tileIndex.x, 0, m_player->getTileMap().getMapSize().x-1);
	tileIndex.y = jl::Math::clamp(tileIndex.y, 0, m_player->getTileMap().getMapSize().y-1);

	// Check x collision
	if(!m_player->getTileMap().getTile(tileIndex).isSolid() || m_player->getTileMap().getTile(tileIndex).getTileType() == 5)
		entity.position.x += speedOffset.x;

	tileIndex = sf::Vector2i(
		entity.position.x / m_player->getTileMap().getTileSize(), 
		(entity.position.y+speedOffset.y) / m_player->getTileMap().getTileSize());

	tileIndex.x = jl::Math::clamp(tileIndex.x, 0, m_player->getTileMap().getMapSize().x-1);
	tileIndex.y = jl::Math::clamp(tileIndex.y, 0, m_player->getTileMap().getMapSize().y-1);

	// Check y collision
	if(!m_player->getTileMap().getTile(tileIndex).isSolid() || m_player->getTileMap().getTile(tileIndex).getTileType() == 5)
		entity.position.y += speedOffset.y;
}