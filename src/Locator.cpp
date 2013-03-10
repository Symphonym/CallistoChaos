#include "Locator.h"

namespace cc
{
	namespace Locator
	{
		// Defining and declaring services
		Renderer *m_renderService = nullptr;
		sf::Event *m_eventService = nullptr;
		Engine *m_engineService = nullptr;


		void provide(Renderer *service){ m_renderService = service; }
		void provide(sf::Event *service){ m_eventService = service; }
		void provide(Engine *service){ m_engineService = service; }

		Renderer *getRender(){ return m_renderService; }
		sf::Event *getEvent(){ return m_eventService; }
		Engine *getEngine(){ return m_engineService; }
	};
};