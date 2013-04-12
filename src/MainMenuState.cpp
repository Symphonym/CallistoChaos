#include "MainMenuState.h"
#include "Utility.h"
#include "Engine.h"
#include "GameState.h"
#include "ParticleManager.h"

MainMenuState::MainMenuState(jl::Engine *engine) :
	jl::State(engine),
	m_selectedItem(0)
{
	m_text.setFont(engine->getAssets().getAsset<jl::FontAsset>("res/Minecraftia.ttf")->get());
	m_text.setCharacterSize(64);

	m_menuItems.push_back("Start");
	m_menuItems.push_back("Exit");

	m_solarView = getEngine()->getWindow().getDefaultView();

	m_backgroundPlanet.setTexture(getEngine()->getAssets().getAsset<jl::TextureAsset>("res/planet2.png")->get());
	m_backgroundPlanet.setScale(
		300.0 / m_backgroundPlanet.getTexture()->getSize().x,
		300.0 / m_backgroundPlanet.getTexture()->getSize().y);
	m_backgroundPlanet.setPosition(
		getEngine()->getWindow().getSize().x/2 - (m_backgroundPlanet.getGlobalBounds().width/2),
		getEngine()->getWindow().getSize().y/2 - (m_backgroundPlanet.getGlobalBounds().height/2));

	for(int i = 0; i < 1500; i++)
	{
		sf::Sprite galaxyItem;
		galaxyItem.setTexture(getEngine()->getAssets().getAsset<jl::TextureAsset>("res/galax.png")->get());
		galaxyItem.setScale(2, 2);
		sf::IntRect textureRect;

		if(std::rand()%100 < 4)
			switch(std::rand()%6)
			{
				case 0:
					textureRect = sf::IntRect(0,0, 6, 6);
				break;
				case 1:
					textureRect = sf::IntRect(7, 0, 7, 7);
				break;
				case 2:
					textureRect = sf::IntRect(18, 0, 6, 6);
				break;
				case 3:
					textureRect = sf::IntRect(25, 0, 7, 7);
				break;
				case 4:
					textureRect = sf::IntRect(35, 0, 12, 12);
				break;
				case 5:
					textureRect = sf::IntRect(48, 0, 10, 10);
				break;
			}
		else
		{
			switch(std::rand()%2)
			{
				case 0:
					textureRect = sf::IntRect(15, 3, 2, 2);
				break;
				case 1:
					textureRect = sf::IntRect(33, 3, 1, 1);
				break;
			}
		}
		galaxyItem.setTextureRect(textureRect);
		galaxyItem.setPosition(
			(-m_solarView.getSize().x) + jl::Math::randDouble(0, m_solarView.getSize().x*3),
			(-m_solarView.getSize().y) + jl::Math::randDouble(0, m_solarView.getSize().y*3));
		m_backgroundGalaxy.push_back(galaxyItem);
	}
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
		{
			ParticleManager::clearParticles();
			getEngine()->getStack().pushState(std::unique_ptr<State>(new GameState(getEngine())));
		}
		else if(buttonName == "Exit")
			getEngine()->getStack().popState();
	}
}
void MainMenuState::update()
{
	m_solarView.rotate(5*getEngine()->getDelta());

	for(int i = 0; i < 50; i++)
		ParticleManager::addParticle(
			sf::Vector2f(
				getEngine()->getWindow().getSize().x/2,
				getEngine()->getWindow().getSize().y/2),
			jl::Math::randDouble(0.2, 1.0),
			jl::Math::randInt(100, 300),
			0,
			jl::Math::randInt(0, 360),
			sf::Color(255, 102, 51),
			sf::Vector2f(30, 30),
			-1,
			true);
	ParticleManager::update(getEngine()->getDelta());
}
void MainMenuState::render()
{
	getEngine()->getWindow().setView(m_solarView);

	for(int i = 0; i < m_backgroundGalaxy.size(); i++)
			getEngine()->getWindow().draw(m_backgroundGalaxy[i]);


	getEngine()->getWindow().setView(getEngine()->getWindow().getDefaultView());
	
	ParticleManager::render(getEngine()->getWindow());
	getEngine()->getWindow().draw(m_backgroundPlanet);

	for(int i = 0; i < m_menuItems.size(); i++)
	{
		sf::Color transparentColor = i == m_selectedItem ? sf::Color(0, 0, 0) : sf::Color(255, 102, 51);
		transparentColor.a = 150;
		m_text.setColor(transparentColor);
		m_text.setString(m_menuItems[i]);
		m_text.setPosition(
			(getEngine()->getWindow().getSize().x*0.5) - m_text.getGlobalBounds().width/2,
			(getEngine()->getWindow().getSize().y*0.4) + i*(m_text.getGlobalBounds().height*1.5));
		getEngine()->getWindow().draw(m_text);
	}
}