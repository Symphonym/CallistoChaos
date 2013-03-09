#include "Locator.h"

namespace cc
{
	// Defining static members
	sf::RenderTarget* Locator::m_renderService;
	sf::Event* Locator::m_eventService;

	void Locator::provide(sf::RenderTarget* service){ Locator::m_renderService = service; }
	void Locator::provide(sf::Event* service){ Locator::m_eventService = service; }

	sf::RenderTarget* Locator::getRender(){ return m_renderService; }
	sf::Event* Locator::getEvent(){ return m_eventService; }
};