#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Engine.h"
#include "TileMap.h"
#include "State.h"
#include "CharacterManager.h"
#include "TileOptionManager.h"

class GameState : public jl::State
{
private:

	// Stores level data
	TileMap m_tileMap;
	// Stores characters on the TileMap
	CharacterManager m_characters;
	// Stores options for different tiletypes
	TileOptionManager m_tileOptions;
	sf::View m_view;


public:

	explicit GameState(jl::Engine *engine);

	void events();
	void update();
	void render();
};

#endif