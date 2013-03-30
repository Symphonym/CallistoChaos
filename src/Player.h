#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <map>
#include "TileCharacter.h"
#include "Weapon.h"
#include "BedControl.h"
#include "Workbench.h"

class TileMap;
class Player : public TileCharacter
{
private:

	void duringWalkRight();
	void duringWalkLeft();
	void duringWalkUp();
	void duringWalkDown();


	std::map<int, std::unique_ptr<Weapon>> m_weapons;
	Workbench m_workbench;

	sf::Text m_resourceText, m_playerText;
	sf::Sprite m_healthSprite, m_ammoSprite;

	// Control for bed
	BedControl m_bedControl;

	// Selected weapon
	int m_selectedWeapon;

public:

	explicit Player(TileMap &tilemap, jl::AssetManager &assets, const sf::Vector2i &tileIndex);

	void events(sf::Event &events);
	void update(double deltaTime);
	void render(sf::RenderTarget &target);

	virtual void characterEvents(TileCharacter::Event events);

	void addWeapon(std::unique_ptr<Weapon> weapon);

	Workbench &getWorkbench();
	BedControl &getBedControl();
	Weapon* getActiveWeapon();
};

#endif