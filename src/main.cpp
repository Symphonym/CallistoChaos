#include "GameEngine.h"
#include "Settings.h"

int main(int argc, char const *args[])
{
	// Create a new game engine object
	// Initialize default settings
	GameEngine game;

	// Allow editing of settings through command line
	game.parseArgs(argc, args);

	// Set non-changeable settings
	cc::Settings::setString("windowTitle", "Callisto Chaos");

	// Run the game engine with default values
	game.run();

	return 0;
}