#ifndef ENGINE_H
#define ENGINE_H

#include <SFML/Graphics.hpp>
#include "Settings.h"
#include "State.h"
#include <memory>

namespace cc
{
	class Engine
	{
	private:

		// State machine data
		std::vector<std::unique_ptr<State>> m_stateStack;
		bool m_stateInit;

		// Engine data
		double m_fps, m_deltaTime;
		sf::Clock m_deltaClock;
		void gameloop();

		// Extra engine data
		sf::RenderWindow m_window;
		Settings m_settings;
		sf::Event m_events;


		// Keep base methods private to prevent them from being called by
		// anyone other than the Engine itself
		// These base methods are run on every state, as a global place of control

		// Handle events, window closing, input etc
		virtual void events(){};
		// Handle updating
		virtual void update(){};
		// Handle rendering
		virtual void render(){};

	public:

		// Default constructor, mainly provides Locator with services and
		// initializes default game settings.
		// Some game settings can be overloaded through commandline commands
		Engine();

		// Initializes the Game engine, and starts the game loop.
		void run(unsigned int width, unsigned int height, const std::string& title, unsigned int style);

	protected:

		// Methods for communicating with the state machine

		// Overwrite any existing states, clear the stack and set new state
		void changeState(std::unique_ptr<State> state);
		// Push new state ontop of the stack
		void pushState(std::unique_ptr<State> state);
		// Pop current state and resume the previous
		void popState();
	};
};

#endif