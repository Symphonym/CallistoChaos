#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Engine.h"
#include "TileMap.h"
#include "State.h"
#include "CharacterManager.h"
#include "TileOptionManager.h"
#include "Workbench.h"
#include "BedControl.h"
#include "LootManager.h"

class GameState : public jl::State
{
private:

	double m_gameRatio;

	// Stores level data
	TileMap m_tileMap;
	// Stores characters on the TileMap
	CharacterManager m_characters;
	// Stores options for different tiletypes
	TileOptionManager m_tileOptions;
	// Stores the loot on the TileMap
	LootManager m_loot;
	
	sf::View m_view;

	// Bed controller
	BedControl m_bedControl;
	// Workbench for crafting/shop
	Workbench m_workbench;

	sf::Text m_scoreText;


public:

	explicit GameState(jl::Engine *engine);

	void events();
	void update();
	void render();

	TileMap &getTileMap();
	CharacterManager &getChars();
	BedControl &getBed();
	Workbench &getWorkbench();
	LootManager &getLoot();
};

#endif