#include "LootManager.h"
#include "Utility.h"
#include "Player.h"
#include "TileMap.h"

LootManager::LootManager(jl::AssetManager &assets) : 
	m_player(nullptr)
{
	m_sprite.setTexture(assets.getAsset<jl::TextureAsset>("res/tiles.png")->get());
	m_sprite.setTextureRect(sf::IntRect(48, 0, 8, 7));
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
	entity.speed = std::rand() % 50;
	entity.sineWave = directionAngle;

	m_entities.push_back(entity);
}
#include <iostream>
void LootManager::update(double deltaTime)
{
	for(int i = 0; i < m_entities.size(); i++)
	{
		// Initial sliding
		sf::Vector2f speedAdd(
			m_entities[i].direction.x*m_entities[i].speed*deltaTime,
			m_entities[i].direction.y*m_entities[i].speed*deltaTime);

		m_entities[i].sineWave += 10*deltaTime;

		if(m_entities[i].speed < 0)
			m_entities[i].speed = 0;
		if(m_entities[i].sineWave > 360)
			m_entities[i].sineWave = 0;

		moveEntity(m_entities[i], speedAdd);
		m_entities[i].speed -= 40*deltaTime;

		sf::Vector2i tileIndex(
			m_entities[i].position.x / m_player->getTileMap().getTileSize(), 
			m_entities[i].position.y / m_player->getTileMap().getTileSize());

		int tileDistance = std::abs(m_player->getIndex().x - tileIndex.x) + std::abs(m_player->getIndex().y - tileIndex.y);
		if(tileDistance < 5)
		{
			sf::Vector2f centerPlayer(
				m_player->getSprite().getPosition().x+m_player->getSprite().getGlobalBounds().width/2,
				m_player->getSprite().getPosition().y+m_player->getSprite().getGlobalBounds().height/2);
			
			//sf::Vector2f invLe(jl::Vec::invLerp(m_entities[i].position, centerPlayer, deltaTime*0.5)*0.001f);
			sf::Vector2f lerpAdd(jl::Vec::lerp(m_entities[i].position, centerPlayer, deltaTime*0.5));
			//m_entities[i].position += invLe;
			//std::cout << invLe.x << std::endl;
			moveEntity(m_entities[i], lerpAdd - m_entities[i].position);

			if(jl::Vec::length(centerPlayer - m_entities[i].position) < 1)
			{
				m_player->addCurrency(1);
				//m_entities.erase(m_entities.begin() + i);
				continue;
			}
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
	if(!m_player->getTileMap().getTile(tileIndex).isSolid())
		entity.position.x += speedOffset.x;

	tileIndex = sf::Vector2i(
		entity.position.x / m_player->getTileMap().getTileSize(), 
		(entity.position.y+speedOffset.y) / m_player->getTileMap().getTileSize());

	tileIndex.x = jl::Math::clamp(tileIndex.x, 0, m_player->getTileMap().getMapSize().x-1);
	tileIndex.y = jl::Math::clamp(tileIndex.y, 0, m_player->getTileMap().getMapSize().y-1);

	// Check y collision
	if(!m_player->getTileMap().getTile(tileIndex).isSolid())
		entity.position.y += speedOffset.y;
}