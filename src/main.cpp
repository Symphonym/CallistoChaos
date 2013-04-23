#include "MainMenuState.h"
#include "Engine.h"
#include <SFML/Graphics.hpp>

int main(int argc, char const *args[])
{
	// Initialize random number generator with TIME as seed
	std::srand(time(NULL));

	// Create a new game engine object
	// Initialize default settings
	jl::Engine engine;

	// Allow editing of settings through command line
	engine.parseArgs(argc, args);

	// Set non-changeable settings
	jl::Settings::setString("windowTitle", "Callisto Chaos");
	jl::Settings::setBool("windowFullscreen", false);
	jl::Settings::setInt("windowHeight", 600);
	jl::Settings::setInt("windowWidth", 800);
	jl::Settings::setInt("windowFpsLimit", 60);

	// Run the game engine with default values
	engine.run<MainMenuState>();

	return 0;
}