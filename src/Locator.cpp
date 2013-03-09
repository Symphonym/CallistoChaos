#include "Locator.h"

namespace cc
{
	// Defining static members
	Settings* Locator::m_settingsService;
	sf::RenderTarget* Locator::m_renderService;
	sf::Event* Locator::m_eventService;

	void Locator::provide(Settings* service){ Locator::m_settingsService = service; }
	void Locator::provide(sf::RenderTarget* service){ Locator::m_renderService = service; }
	void Locator::provide(sf::Event* service){ Locator::m_eventService = service; }

	Settings* Locator::getSettings(){ return m_settingsService; }
	sf::RenderTarget* Locator::getRender(){ return m_renderService; }
	sf::Event* Locator::getEvent(){ return m_eventService; }
};