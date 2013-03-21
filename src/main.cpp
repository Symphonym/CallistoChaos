#include "GameState.h"
#include "Engine.h"
#include "Settings.h"
#include <SFML/Graphics.hpp>

int main(int argc, char const *args[])
{
	// Create a new game engine object
	// Initialize default settings
	jl::Engine engine;

	// Allow editing of settings through command line
	engine.parseArgs(argc, args);

	// Set non-changeable settings
	jl::Settings::setString("windowTitle", "Callisto Chaos");

	// Run the game engine with default values
	engine.run<GameState>();

	return 0;
}