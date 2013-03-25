#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "TileCharacter.h"
#include "Weapon.h"

class TileMap;
class Player : public TileCharacter
{
private:

	void duringWalkRight();
	void duringWalkLeft();
	void duringWalkUp();
	void duringWalkDown();

	std::vector<std::unique_ptr<Weapon>> m_weapons;

	sf::Text m_resourceText, m_playerText;
	sf::Sprite m_healthSprite, m_ammoSprite;

	// Selected weapon
	int m_selectedWeapon;

public:

	explicit Player(TileMap &tilemap, jl::AssetManager &assets, const sf::Vector2i &tileIndex);

	void events(sf::Event &events);
	void update(double deltaTime);
	void render(sf::RenderTarget &target);

	Weapon* getActiveWeapon();
};

#endif