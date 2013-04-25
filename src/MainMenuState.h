#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include <SFML/Graphics.hpp>
#include "State.h"

class MainMenuState : public jl::State
{
private:

	sf::Text m_text, m_scoreText;
	int m_selectedItem;
	std::vector<std::string> m_menuItems;

	sf::Sprite m_backgroundPlanet;
	std::vector<sf::Sprite> m_backgroundGalaxy;
	sf::View m_solarView;

public:

	explicit MainMenuState(jl::Engine *engine);

	void events();
	void update();
	void render();
};

#endif