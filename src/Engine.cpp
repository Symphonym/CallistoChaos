#include "Engine.h"
#include "Settings.h"
#include "Utility.h"
#include "EngineInfo.h"

namespace cc
{
	Engine::Engine()
	 : 
	 m_stateInit(false),
	 m_fps(0),
	 m_deltaTime(0)
	{
		// Initialize default settings
		Settings::setString("infoAuthor", "Programmed by Jakob Larsson 8/3-2013, aged 17");
		Settings::setString("infoOS", "Supports: Linux and Windows");
		Settings::setString("infoName", "Callisto Chaos");
		Settings::setString("windowTitle", "--> Jakob Larsson Game Engine <--");
		Settings::setBool("windowShowCursor", true);
		Settings::setBool("windowVsync", false);
		Settings::setInt("windowFpsLimit", 0);
		Settings::setInt("windowWidth", 800);
		Settings::setInt("windowHeight", 600);
		Settings::setInt("windowStyle", sf::Style::Close);
		Settings::setInt("gameGlobalVolume", 100);

		// Initialize data
		m_renderer.setRenderTarget(m_window);

		// Checks if there is an active Engine, if not, makes THIS Engine the active one
		if(EngineInfo::m_engine == nullptr)
			setActive();
	}	


	void Engine::run()
	{
		// Only allow running this method if the game isn't already running
		if(!m_window.isOpen())
		{
			// Initialize renderwindow and load settings
			m_window.create(
				sf::VideoMode(Settings::getInt("windowWidth"), Settings::getInt("windowHeight")),
				 Settings::getString("windowTitle"),
				 Settings::getInt("windowStyle"));
			m_window.setFramerateLimit(Settings::getInt("windowFpsLimit"));
			m_window.setMouseCursorVisible(Settings::getBool("windowShowCursor"));
			m_window.setVerticalSyncEnabled(Settings::getBool("windowVsync"));
			sf::Listener::setGlobalVolume(Settings::getInt("gameGlobalVolume"));

			// Initialize additional game engine data
			init();

			// Start gameloop
			gameloop();
		}
	}
	void Engine::gameloop()
	{
		while(m_window.isOpen())
		{
			m_deltaTime = m_deltaClock.restart().asSeconds();
			m_fps = 1.0 / m_deltaTime;

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

				// Global events
				events();
			}

			// Handle updating in current state
			if(m_stateInit)
				m_stateStack.back()->update();

			// Global update
			update();


			m_window.clear();

			// Handle rendering in current state
			if(m_stateInit)
				m_stateStack.back()->render();

			// Global rendering
			render();

			m_window.display();
		}
	}

	void Engine::setActive()
	{
		EngineInfo::m_engine = this;
	}

	void Engine::parseArgs(int argc, char const *args[])
	{
		int argtype = -1;

		for(int i = 0; i < argc; i++)
		{

			std::string arg = args[i];

			if(arg == "--int")
				argtype = 0;
			else if(arg == "--double")
				argtype = 1;
			else if(arg == "--bool")
				argtype = 2;
			else if(arg == "--string")
				argtype = 3;
			else
			{
				// Just scan the argument for an equal sign and attempt to parse data from it
				if(argtype != -1 && arg.find('=') != std::string::npos)
				{
					// Splits the argument to name and value, from the form: "name=value"
					std::string name = arg.substr(0, arg.find('='));
					std::string value = arg.substr(arg.find('=')+1, arg.size()+1);

					if(argtype == 0) // Integer settings
						Settings::setInt(name, Util::convertData<std::string, int>(value));
					else if(argtype == 1) // Double settings
						Settings::setDouble(name, Util::convertData<std::string, double>(value));
					else if(argtype == 2) // Bool settings
						Settings::setBool(name, Util::convertData<std::string, bool>(value));
					else if(argtype == 3) // String settings
						Settings::setString(name, value);

					argtype = -1;
				}
				else
					continue;
			}

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