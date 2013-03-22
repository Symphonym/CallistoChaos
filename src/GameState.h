#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Engine.h"
#include "TileMap.h"
#include "State.h"
#include "CharacterManager.h"

class GameState : public jl::State
{
private:

	TileMap m_tileMap;
	CharacterManager m_characters;

public:

	sf::Texture texture;
	sf::Sprite sprite;
	sf::View view;
	jl::FrameAnimation anim;

	explicit GameState(jl::Engine *engine);

	void events();
	void update();
	void render();
};

#endif