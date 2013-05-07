#ifndef LOOTMANAGER_H
#define LOOTMANAGER_H

#include <SFML/Graphics.hpp>
#include "AssetManager.h"

class Player;
class LootManager
{
private:

	Player *m_player;
	sf::Sprite m_sprite;

	struct LootEntity
	{
		// Position of the entity and the direction it's moving
		sf::Vector2f position, direction;

		// Speed and sine wave value
		float slideSpeed, speed, sineWave;

		// Alpha fading, as the entity goes away if not looted
		float alphaFade;

		// Type of entity, "Ammo" or "Currency"
		std::string type;
	};

	std::vector<LootEntity> m_entities;

	void moveEntity(LootEntity &entity, const sf::Vector2f &speedOffset);

public:

	explicit LootManager(jl::AssetManager &assets);
	explicit LootManager();

	void providePlayer(Player *player);

	void spawnEntity(const sf::Vector2f &position, const std::string &entityType);

	void update(double deltaTime);
	void render(sf::RenderTarget &target);
};

#endif