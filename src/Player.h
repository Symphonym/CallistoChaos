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

	static const int maxCurrency = 300;
	static const int maxAmmo = 300;

	std::vector<std::unique_ptr<Weapon>> m_weapons;

	sf::Text m_resourceText;

	// How much currency the player has
	int m_currencyAmount;
	// How much ammo the plyaer has
	int m_ammoAmount;

public:

	explicit Player(TileMap &tilemap, jl::AssetManager &assets, const sf::Vector2i &tileIndex);

	void events(sf::Event &events);
	void update(double deltaTime);
	void render(sf::RenderTarget &target);

	bool pay(int currency);
};

#endif