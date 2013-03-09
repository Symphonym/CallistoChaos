#include "Engine.h"
#include "Locator.h"

namespace cc
{
	Engine::Engine()
	 : 
	 m_stateInit(false),
	 m_fps(0),
	 m_deltaTime(0)
	{
		// Provide locator with services
		Locator::provide(&m_settings);
		Locator::provide(&m_window);
		Locator::provide(&m_events);

		// Initialize default settings
		Locator::getSettings()->setString("infoAuthor", "Programmed by Jakob Larsson 8/3-2013, aged 17");
		Locator::getSettings()->setString("infoOS", "Supports: Linux and Windows");
		Locator::getSettings()->setString("infoName", "Callisto Chaos");
		Locator::getSettings()->setString("windowTitle", "--> Jakob Larsson Game Engine <--");
		Locator::getSettings()->setBool("windowShowCursor", true);
		Locator::getSettings()->setBool("windowVsync", false);
		Locator::getSettings()->setInt("windowFpsLimit", 0);
		Locator::getSettings()->setInt("windowWidth", 800);
		Locator::getSettings()->setInt("windowHeight", 600);
		Locator::getSettings()->setInt("windowStyle", sf::Style::Close);
	}	

	void Engine::run(unsigned int width, unsigned int height, const std::string& title, unsigned int style)
	{
		// Initialize renderwindow and load extra settings
		m_window.create(
			sf::VideoMode(Locator::getSettings()->getInt("windowWidth"), Locator::getSettings()->getInt("windowHeight")),
			 Locator::getSettings()->getString("windowTitle"),
			 Locator::getSettings()->getInt("windowStyle"));
		m_window.setFramerateLimit(Locator::getSettings()->getInt("windowFpsLimit"));
		m_window.setMouseCursorVisible(Locator::getSettings()->getBool("windowShowCursor"));
		m_window.setVerticalSyncEnabled(Locator::getSettings()->getBool("windowVsync"));

		// Start gameloop
		gameloop();
	}
	void Engine::gameloop()
	{
		while(m_window.isOpen())
		{
			m_deltaTime = m_deltaClock.restart().asSeconds();
			m_fps = 1.0 / m_deltaTime;
			Locator::getSettings()->setInt("gameFps", m_fps);

			// Initialize the uninitialized state, if there's one
			if(!m_stateInit && m_stateStack.size() > 0)
			{
				// Handle extra initialzation of the current state
				// Default initialization should be handled in the ctor
				m_stateStack.back()->init();
				m_stateInit = true;
			}


			while(m_window.pollEvent(m_events))
			{
				if(m_events.type == sf::Event::Closed ||
					(sf::Keyboard::isKeyPressed(sf::Keyboard::J) &&
					 sf::Keyboard::isKeyPressed(sf::Keyboard::A) &&
					 sf::Keyboard::isKeyPressed(sf::Keyboard::K) &&
					 sf::Keyboard::isKeyPressed(sf::Keyboard::O) &&
					 sf::Keyboard::isKeyPressed(sf::Keyboard::B)))
					m_window.close();

				// Handle events in current state
				if(m_stateInit)
					m_stateStack.back()->events();
			}

			// Handle updating in current state
			if(m_stateInit)
				m_stateStack.back()->update();


			m_window.clear();

			// Handle rendering in current state
			if(m_stateInit)
				m_stateStack.back()->render();

			m_window.display();
		}
	}

	void Engine::changeState(std::unique_ptr<State> state)
	{
		while(m_stateStack.size() > 0)
			m_stateStack.pop_back();

		pushState(std::move(state));
	}
	void Engine::pushState(std::unique_ptr<State> state)
	{
		m_stateInit = true;
		m_stateStack.push_back(std::move(state));
	}
	void Engine::popState()
	{
		m_stateStack.pop_back();

		if(m_stateStack.size() <= 0)
			m_stateInit = false;
	}
};