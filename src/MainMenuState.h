#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include <SFML/Graphics.hpp>
#include "State.h"

class MainMenuState : public jl::State
{
private:

	sf::Text m_text;
	int m_selectedItem;
	std::vector<std::string> m_menuItems;

public:

	explicit MainMenuState(jl::Engine *engine);

	void events();
	void update();
	void render();
};

#endif