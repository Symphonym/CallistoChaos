#ifndef JL_ENGINE_H
#define JL_ENGINE_H

#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include "StateManager.h"
#include "Settings.h"
#include <vector>

namespace jl
{
	class Engine
	{
	private:

		// Engine data
		int m_fps;
		double m_delta;
		sf::Clock m_deltaClock;
		void gameloop();

		// Extra engine data
		sf::RenderWindow m_window;
		sf::Event m_event;
		AssetManager m_assets;
		StateManager m_stack;

		// Keep base methods private to prevent them from being called by
		// anyone other than the Engine itself
		// These base methods are run on every state, as a global place of control

		// Handle initialization, at this point the Engine is
		// guaranteed to have it's initial data initialized
		virtual void init(){};
		// Handle events, window closing etc
		virtual void events(){};
		// Handle updating
		virtual void update(){};
		// Handle rendering
		virtual void render(){};

	public:

		// "Engine" constructor provides the game with default settings,
		// most of which can be overloaded.
		explicit Engine();
		virtual ~Engine();

		// Reloads all settings from the global jl::Settings data
		void refreshSettings();

		// "run" initializes the Game engine and starts the game loop. The
		// specified type T is the startup State that will be used.
		template<typename T> void run(const std::string iconPath = "", unsigned int iconWidth = 0, unsigned int iconHeight = 0)
		{
			// Only allow running this method if the game isn't already running
			if(!m_window.isOpen())
			{
				// Initialize renderwindow and load settings
				refreshSettings();

				// Initialize startup state with type T
				std::unique_ptr<State> startupState = std::unique_ptr<State>(new T(this));
				m_stack.pushState(std::move(startupState));

				// Initialize additional game engine data
				init();

				if(!iconPath.empty() && iconWidth != 0 && iconHeight != 0)
				{
					sf::Image icon;
					icon.loadFromFile(iconPath);
					m_window.setIcon(iconWidth, iconHeight, icon.getPixelsPtr());
				}

				// Start gameloop
				gameloop();
			}
		};

		// "parseArgs" allows Game settings to be set through command-line
		// in the format of: settingName=value
		void parseArgs(int argc, char const* args[]);
		void parseArgs(const std::vector<std::string> &args);


		void render(const sf::Drawable &drawable, const sf::RenderStates &states = sf::RenderStates::Default);
		void render(const sf::Vertex *vertices, unsigned int vertexCount, sf::PrimitiveType type, const sf::RenderStates &states = sf::RenderStates::Default);

		// "createView" is a factory function using the sf::RenderWindow's
		// default and the specified zoomFactor to return a custom sf::View.
		sf::View createView(double zoomFactor = 1.0);

		double getDelta() const;
		int getFps() const;

		sf::RenderWindow &getWindow();
		sf::Event &getEvent();
		AssetManager &getAssets();
		StateManager &getStack();
	};
};

#endif
