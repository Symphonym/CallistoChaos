#ifndef CC_GAMEENGINE_H
#define CC_GAMEENGINE_H

#include "Engine.h"
#include <SFML/Graphics.hpp>

namespace cc
{
	class GameEngine : public Engine
	{
	public:

		sf::Texture texture;
		sf::Sprite sprite;
		sf::View view;

		explicit GameEngine();

		void events();
		void update();
		void render();
	};
};

#endif