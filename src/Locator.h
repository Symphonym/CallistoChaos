#ifndef CC_LOCATOR_H
#define CC_LOCATOR_H

#include "Settings.h"
#include <SFML/Graphics.hpp>

namespace cc
{
	class Locator
	{
	private:
		static sf::RenderTarget* m_renderService;
		static sf::Event* m_eventService;

	public:
		static void provide(sf::RenderTarget* service);
		static void provide(sf::Event* service);

		static sf::RenderTarget* getRender();
		static sf::Event* getEvent();
	};
};

#endif