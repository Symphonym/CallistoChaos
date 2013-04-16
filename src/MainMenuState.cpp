#include "MainMenuState.h"
#include "Utility.h"
#include "Engine.h"
#include "GameState.h"
#include "ParticleManager.h"
#include "GalaxyGenerator.h"

MainMenuState::MainMenuState(jl::Engine *engine) :
	jl::State(engine),
	m_selectedItem(0)
{
	m_text.setFont(engine->getAssets().getAsset<jl::FontAsset>("res/Minecraftia.ttf")->get());
	m_text.setCharacterSize(64);

	m_menuItems.push_back("Start");
	m_menuItems.push_back("Exit");

	m_solarView = getEngine()->getWindow().getDefaultView();
	sf::Vector2i mapSize(24, 20);
	int tileSize = 16;

	m_solarView.setSize(mapSize.x*tileSize, mapSize.y*tileSize);
	m_solarView.setCenter((mapSize.x*tileSize)/2, (mapSize.y*tileSize)/2);
	m_solarView.zoom(10);

	m_backgroundPlanet.setTexture(getEngine()->getAssets().getAsset<jl::TextureAsset>("res/planet2.png")->get());
	m_backgroundPlanet.setScale(
		1000.0 / m_backgroundPlanet.getTexture()->getSize().x,
		1000.0 / m_backgroundPlanet.getTexture()->getSize().y);

	GalaxyGenerator::setView(m_solarView);
	GalaxyGenerator::setPlanetTextureSheet(getEngine()->getAssets().getAsset<jl::TextureAsset>("res/galax.png")->get());
	GalaxyGenerator::addPlanet(4,sf::IntRect(0,0,6,6), -m_solarView.getSize(), m_solarView.getSize()*3.f);
	GalaxyGenerator::addPlanet(4,sf::IntRect(7,0,7,7), -m_solarView.getSize(), m_solarView.getSize()*3.f);
	GalaxyGenerator::addPlanet(4,sf::IntRect(18,0,6,6), -m_solarView.getSize(), m_solarView.getSize()*3.f);
	GalaxyGenerator::addPlanet(4,sf::IntRect(25,0,7,7), -m_solarView.getSize(), m_solarView.getSize()*3.f);
	GalaxyGenerator::addPlanet(4,sf::IntRect(35,0,12,12), -m_solarView.getSize(), m_solarView.getSize()*3.f);
	GalaxyGenerator::addPlanet(4,sf::IntRect(48,0,10,10), -m_solarView.getSize(), m_solarView.getSize()*3.f);
	GalaxyGenerator::addPlanet(100,sf::IntRect(15,3,2,2), -m_solarView.getSize(), m_solarView.getSize()*3.f);
	GalaxyGenerator::addPlanet(100,sf::IntRect(33,3,1,1), -m_solarView.getSize(), m_solarView.getSize()*3.f);
	GalaxyGenerator::generateGalaxy(1500, sf::Vector2f(4, 4));
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
	m_backgroundPlanet.setPosition(
		m_solarView.getCenter().x - m_backgroundPlanet.getGlobalBounds().width/2,
		m_solarView.getCenter().y - m_backgroundPlanet.getGlobalBounds().height/2);

	int speed = jl::Math::randInt(700, 900);
	double stopTime = 1.4;
	double lifeTime = jl::Math::randDouble(1.0, 1.4);
	if(jl::Math::randDouble(0, 100) < 1.0)
	{
		speed = jl::Math::randInt(3000, 5000);
		stopTime = -1;
		lifeTime = jl::Math::randDouble(2.0, 3.0);
	}

	for(int i = 0; i < 5; i++)
		ParticleManager::addParticle(
			m_solarView.getCenter(),
			jl::Math::randDouble(1.0, 1.4),
			jl::Math::randInt(700, 900),
			0,
			jl::Math::randInt(0, 360),
			sf::Color(255, 102, 51),
			sf::Vector2f(m_backgroundPlanet.getScale().x, m_backgroundPlanet.getScale().y),
			stopTime,
			true);

	GalaxyGenerator::rotate(getEngine()->getDelta()*5);
	ParticleManager::update(getEngine()->getDelta());
}
void MainMenuState::render()
{
	getEngine()->getWindow().setView(m_solarView);
	ParticleManager::render(getEngine()->getWindow());

	GalaxyGenerator::render(getEngine()->getWindow());
	getEngine()->getWindow().draw(m_backgroundPlanet);

	// Get default view when rendering text
	getEngine()->getWindow().setView(getEngine()->getWindow().getDefaultView());
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