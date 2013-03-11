#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "Engine.h"
#include <SFML/Graphics.hpp>
#include "FrameAnimation.h"

class GameEngine : public cc::Engine
{
public:

	sf::Texture texture;
	sf::Sprite sprite;
	sf::View view;
	cc::FrameAnimation anim;

	explicit GameEngine();

	void init();
	void events();
	void update();
	void render();
};

#endif