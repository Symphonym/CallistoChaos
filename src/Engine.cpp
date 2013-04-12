#include "Engine.h"
#include "Utility.h"
#include "SoundManager.h"

namespace jl
{
	Engine::Engine()
	 : 
	 m_fps(0),
	 m_delta(0),
	 m_deltaClock(),
	 m_window(),
	 m_assets(),
	 m_stack()
	{
		// Initialize default settings
		Settings::setString("infoAuthor", "Programmed by Jakob Larsson 8/3-2013, aged 17");
		Settings::setString("infoOS", "Supports: Linux and Windows");
		Settings::setString("infoName", "Jakob Larsson Game Engine");
		Settings::setString("windowTitle", "--> Jakob Larsson Game Engine <--");
		Settings::setBool("windowShowCursor", true);
		Settings::setBool("windowVsync", false);
		Settings::setBool("windowFullscreen", false);
		Settings::setInt("windowFpsLimit", 0);
		Settings::setInt("windowWidth", 800);
		Settings::setInt("windowHeight", 600);
		Settings::setInt("gameGlobalVolume", 100);

		// Register Engine data to those who need it
		SoundManager::registerAssets(&m_assets);
	}	

	void Engine::gameloop()
	{
		while(m_window.isOpen())
		{
			m_delta = m_deltaClock.restart().asSeconds();
			m_fps = 1 / m_delta;

			// Limit deltaTime to avoid "jumpy" behaviour
			if(m_delta > 0.01)
				m_delta = 0.01;

			// Do delete requests
			m_stack.processDeletes();

			// Finish loop if Stack is empty
			if(m_stack.isEmpty())
			{
				m_window.close();
				break;
			}

			while(m_window.pollEvent(m_event))
			{
				if(m_event.type == sf::Event::Closed ||
					(sf::Keyboard::isKeyPressed(sf::Keyboard::J) &&
					 sf::Keyboard::isKeyPressed(sf::Keyboard::A) &&
					 sf::Keyboard::isKeyPressed(sf::Keyboard::K) &&
					 sf::Keyboard::isKeyPressed(sf::Keyboard::O) &&
					 sf::Keyboard::isKeyPressed(sf::Keyboard::B)))
					m_window.close();
				else if(m_event.type == sf::Event::JoystickConnected)
					sf::Joystick::update();

				// State events
				m_stack.getActiveState()->events();

				// Global events
				events();
			}

			// Finish loop if Stack is empty
			if(m_stack.isEmpty())
			{
				m_window.close();
				break;
			}

			// State updating
			m_stack.getActiveState()->update();

			// Global update
			update();

			m_window.clear();

			// Finish loop if Stack is empty
			if(m_stack.isEmpty())
			{
				m_window.close();
				break;
			}

			// State rendering
			m_stack.getActiveState()->render();

			// Global rendering
			render();


			m_window.display();
		}
	}

	void Engine::parseArgs(int argc, char const *args[])
	{
		std::vector<std::string> vecArgs;
		for(int i = 0; i < argc; i++)
			vecArgs.push_back(std::string(args[i]));

		parseArgs(vecArgs);
		/*int argtype = -1;

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

		}*/
	}
	void Engine::parseArgs(const std::vector<std::string> &args)
	{
		int argtype = -1;

		for(int i = 0; i < args.size(); i++)
		{

			if(args[i] == "--int")
				argtype = 0;
			else if(args[i] == "--double")
				argtype = 1;
			else if(args[i] == "--bool")
				argtype = 2;
			else if(args[i] == "--string")
				argtype = 3;
			else
			{
				// Just scan the argument for an equal sign and attempt to parse data from it
				if(argtype != -1 && args[i].find('=') != std::string::npos)
				{
					// Splits the argument to name and value, from the form: "name=value"
					std::string name = args[i].substr(0, args[i].find('='));
					std::string value = args[i].substr(args[i].find('=')+1, args[i].size()+1);

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

	void Engine::refreshSettings()
	{
		// Initialize renderwindow and load settings
		unsigned int windowStyle = Settings::getBool("windowFullscreen") == true ? sf::Style::Fullscreen : sf::Style::Close;
		if(windowStyle == sf::Style::Fullscreen)
		{
			Settings::setInt("windowWidth", sf::VideoMode::getDesktopMode().width);
			Settings::setInt("windowHeight", sf::VideoMode::getDesktopMode().height);
		}

		m_window.create(
			sf::VideoMode(Settings::getInt("windowWidth"), Settings::getInt("windowHeight")),
			 Settings::getString("windowTitle"),
			 windowStyle);
		m_window.setFramerateLimit(Settings::getInt("windowFpsLimit"));
		m_window.setMouseCursorVisible(Settings::getBool("windowShowCursor"));
		m_window.setVerticalSyncEnabled(Settings::getBool("windowVsync"));
		sf::Listener::setGlobalVolume(Settings::getInt("gameGlobalVolume"));
	}

	void Engine::render(const sf::Drawable &drawable, const sf::RenderStates &states)
	{
		m_window.draw(drawable, states);
	}
	void Engine::render(const sf::Vertex *vertices, unsigned int vertexCount, sf::PrimitiveType type, const sf::RenderStates &states)
	{
		m_window.draw(vertices, vertexCount, type, states);
	}

	sf::View Engine::createView(double zoomFactor)
	{
		sf::View view(m_window.getDefaultView());
		view.zoom(zoomFactor);
		view.setCenter(view.getSize().x/2.0, view.getSize().y/2.0);

		return view;
	}


	int Engine::getFps() const
	{
		return m_fps;
	}
	double Engine::getDelta() const
	{
		return m_delta;
	}

	sf::RenderWindow &Engine::getWindow()
	{
		return m_window;
	}
	sf::Event &Engine::getEvent()
	{
		return m_event;
	}
	AssetManager &Engine::getAssets()
	{
		return m_assets;
	}
	StateManager &Engine::getStack()
	{
		return m_stack;
	}
};