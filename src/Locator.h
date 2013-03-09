#ifndef LOCATOR_H
#define LOCATOR_H

#include "Settings.h"
#include <SFML/Graphics.hpp>

namespace cc
{
	class Locator
	{
	private:
		static Settings* m_settingsService;
		static sf::RenderTarget* m_renderService;
		static sf::Event* m_eventService;

	public:
		static void provide(Settings* service);
		static void provide(sf::RenderTarget* service);
		static void provide(sf::Event* service);

		static Settings* getSettings();
		static sf::RenderTarget* getRender();
		static sf::Event* getEvent();
	};
};

#endif