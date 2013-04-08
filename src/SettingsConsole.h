#ifndef SETTINGSCONSOLE_H
#define SETTINGSCONSOLE_H

#include <SFML/Graphics.hpp>

class GameState;
namespace SettingsConsole
{
	// Background color of the console
	void setColor(const sf::Color &color);
	// Set font of the console text
	void setFontData(const sf::Font &font, int fontSize);

	void events(sf::Event &events, GameState &gameState);
	void render(sf::RenderTarget &target); 
};

#endif