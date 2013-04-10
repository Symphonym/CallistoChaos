#include "MainMenuState.h"
#include "Utility.h"
#include "Engine.h"
#include "GameState.h"

MainMenuState::MainMenuState(jl::Engine *engine) :
	jl::State(engine),
	m_selectedItem(0)
{
	m_text.setFont(engine->getAssets().getAsset<jl::FontAsset>("res/Minecraftia.ttf")->get());
	m_text.setCharacterSize(64);
	m_menuItems.push_back("Start");
	m_menuItems.push_back("Exit");
}

void MainMenuState::events()
{
	if(jl::Input::isAxisDown(getEngine()->getEvent(), sf::Joystick::Y, -100))
		--m_selectedItem;
	else if(jl::Input::isAxisDown(getEngine()->getEvent(), sf::Joystick::Y, 100))
		++m_selectedItem;

	if(m_selectedItem < 0)
		m_selectedItem = 0;
	else if (m_selectedItem >= m_menuItems.size())
		m_selectedItem = m_menuItems.size()-1;

	if(jl::Input::isButtonDown(getEngine()->getEvent(), 0))
	{
		std::string buttonName(m_menuItems[m_selectedItem]);

		if(buttonName == "Start")
			getEngine()->getStack().pushState(std::unique_ptr<State>(new GameState(getEngine())));
		else if(buttonName == "Exit")
			getEngine()->getStack().popState();
	}
}
void MainMenuState::update()
{

}
void MainMenuState::render()
{
	for(int i = 0; i < m_menuItems.size(); i++)
	{
		sf::View tempView(getEngine()->getWindow().getView());
		getEngine()->getWindow().setView(getEngine()->getWindow().getDefaultView());

		m_text.setColor(i == m_selectedItem ? sf::Color::Yellow : sf::Color::White);
		m_text.setString(m_menuItems[i]);
		m_text.setPosition(
			(getEngine()->getWindow().getSize().x*0.5) - m_text.getGlobalBounds().width/2,
			(getEngine()->getWindow().getSize().y*0.2) + i*(m_text.getGlobalBounds().height*1.5));
		getEngine()->getWindow().draw(m_text);

		getEngine()->getWindow().setView(tempView);
	}
}