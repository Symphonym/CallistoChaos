#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "State.h"
#include <SFML/Graphics.hpp>
#include "FrameAnimation.h"
#include "Engine.h"

class GameState : public jl::State
{
public:

	sf::Texture texture;
	sf::Sprite sprite;
	sf::View view;
	jl::FrameAnimation anim;

	explicit GameState(jl::Engine *engine);

	void init();
	void events();
	void update();
	void render();
};

#endif