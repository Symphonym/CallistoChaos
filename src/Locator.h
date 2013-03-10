#ifndef CC_LOCATOR_H
#define CC_LOCATOR_H

#include <SFML/Graphics.hpp>
#include "Engine.h"
#include "Renderer.h"

namespace cc
{
	namespace Locator
	{
		void provide(Renderer *service);
		void provide(sf::Event *service);
		void provide(Engine *service);

		Renderer *getRender();
		sf::Event *getEvent();
		Engine *getEngine();
	};
};

#endif