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
		Settings::setInt("windowFpsLimit", 0);
		Settings::setInt("windowWidth", 800);
		Settings::setInt("windowHeight", 600);
		Settings::setInt("windowStyle", sf::Style::Close);
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

				// State events
				m_stack.getActiveState()->events();

				// Global events
				events();
			}

			// State updating
			m_stack.getActiveState()->update();

			// Global update
			update();

			m_window.clear();

			// State rendering
			m_stack.getActiveState()->render();

			// Global rendering
			render();


			m_window.display();
		}
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
		view.setCenter(view.getSize().x/2, view.getSize().y/2);

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
};