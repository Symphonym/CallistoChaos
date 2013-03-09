#include <string>
#include <sstream>
#include <iostream>
#include "GameEngine.h"
#include "Locator.h"


template<typename datatype> datatype convertArg(const std::string& arg)
{
	std::stringstream ss;
	ss << arg;

	datatype data;
	ss >> data;

	if(ss.fail())
		std::cerr << "The value \"" + arg + "\" is not an acceptable value for this setting." << std::endl;
	
	return data;
}
// Parse command line arguments to edit global settings, for funs
void parseArgs(int argc, char const *args[])
{
	int argtype = -1;

	for(int i = 0; i < argc; i++)
	{

		std::cout << "Arg: " << args[i] << std::endl;
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
			if(argtype != -1 && arg.find('=') != std::string::npos)
			{
				std::string name = arg.substr(0, arg.find('='));
				std::string value = arg.substr(arg.find('=')+1, arg.size()+1);

				if(argtype == 0) // Integer settings
					cc::Locator::getSettings()->setInt(name, convertArg<int>(value));
				else if(argtype == 1) // Double settings
					cc::Locator::getSettings()->setDouble(name, convertArg<double>(value));
				else if(argtype == 2) // Bool settings
					cc::Locator::getSettings()->setBool(name, convertArg<bool>(value));
				else if(argtype == 3) // String settings
					cc::Locator::getSettings()->setString(name, value);

				argtype = -1;
			}
			else
				continue;
		}

	}
}

int main(int argc, char const *args[])
{
	// Create a new game engine object
	// Initialize default settings
	cc::GameEngine game;

	// Allow to edit settings through command line
	parseArgs(argc, args);

	// Run the game engine with default values
	game.run(800, 600, "Callisto Chaos", sf::Style::Close);

	return 0;
}