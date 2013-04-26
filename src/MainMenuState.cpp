#include "MainMenuState.h"
#include "Utility.h"
#include "Engine.h"
#include "GameState.h"
#include "ParticleManager.h"
#include "GalaxyGenerator.h"
#include "SoundManager.h"
#include "Settings.h"
#include <fstream>

MainMenuState::MainMenuState(jl::Engine *engine) :
	jl::State(engine),
	m_selectedItem(0),
	m_movedJoystick(false)
{
	m_text.setFont(engine->getAssets().getFont("res/Minecraftia.ttf"));
	m_text.setCharacterSize(54);

	m_scoreText.setFont(engine->getAssets().getFont("res/Minecraftia.ttf"));
	m_scoreText.setCharacterSize(24);


	m_menuItems.push_back("Start");
	m_menuItems.push_back("Exit");
	m_menuItems.push_back("HERO");

	m_solarView = getEngine()->getWindow().getDefaultView();
	sf::Vector2i mapSize(24, 20);
	int tileSize = 16;

	m_solarView.setSize(mapSize.x*tileSize, mapSize.y*tileSize);
	m_solarView.setCenter((mapSize.x*tileSize)/2, (mapSize.y*tileSize)/2);
	m_solarView.zoom(10);

	m_backgroundPlanet.setTexture(getEngine()->getAssets().getTexture("res/planet.png"));
	m_backgroundPlanet.setScale(
		1000.0 / m_backgroundPlanet.getTexture()->getSize().x,
		1000.0 / m_backgroundPlanet.getTexture()->getSize().y);

	m_titleSprite.setTexture(getEngine()->getAssets().getTexture("res/logo.png"));
	m_titleSprite.setScale(5, 5);

	jl::SoundManager::addSound("res/menuSelect.wav");

	GalaxyGenerator::setView(m_solarView);
	GalaxyGenerator::setPlanetTextureSheet(getEngine()->getAssets().getTexture("res/galax.png"));
	GalaxyGenerator::addPlanet(4,sf::IntRect(0,0,6,6), -m_solarView.getSize(), m_solarView.getSize()*3.f);
	GalaxyGenerator::addPlanet(4,sf::IntRect(7,0,7,7), -m_solarView.getSize(), m_solarView.getSize()*3.f);
	GalaxyGenerator::addPlanet(4,sf::IntRect(18,0,6,6), -m_solarView.getSize(), m_solarView.getSize()*3.f);
	GalaxyGenerator::addPlanet(4,sf::IntRect(25,0,7,7), -m_solarView.getSize(), m_solarView.getSize()*3.f);
	GalaxyGenerator::addPlanet(4,sf::IntRect(35,0,12,12), -m_solarView.getSize(), m_solarView.getSize()*3.f);
	GalaxyGenerator::addPlanet(4,sf::IntRect(48,0,10,10), -m_solarView.getSize(), m_solarView.getSize()*3.f);
	GalaxyGenerator::addPlanet(100,sf::IntRect(15,3,2,2), -m_solarView.getSize(), m_solarView.getSize()*3.f);
	GalaxyGenerator::addPlanet(100,sf::IntRect(33,3,1,1), -m_solarView.getSize(), m_solarView.getSize()*3.f);
	GalaxyGenerator::generateGalaxy(1500, sf::Vector2f(4, 4));


	// Set default previous score
	jl::Settings::setInt("gameScore", 0);

	std::ifstream input("res/highscore.encrypted", std::ifstream::in);
	std::string highscoreLine = "";
	input >> highscoreLine;
	input.close();

	if(highscoreLine.empty() || highscoreLine.find("score=") == std::string::npos || highscoreLine.length() <= 6)
	{
		std::ofstream output("res/highscore.encrypted", std::ofstream::trunc);
		output << "score=0";
		output.close();

		jl::Settings::setInt("gameHighscore", 0);
	}
	else
	{
		jl::Settings::setInt(
			"gameHighscore", 
			jl::Util::convertData<std::string, int>(highscoreLine.substr(highscoreLine.find('=') + 1, highscoreLine.length())));

	}
}

void MainMenuState::events()
{
	if(!m_movedJoystick)
	{
		if(sf::Joystick::getAxisPosition(0, sf::Joystick::Y) == -100 &&
			getEngine()->getEvent().type == sf::Event::JoystickMoved)
		{
			--m_selectedItem;
			jl::SoundManager::playSound("res/menuSelect.wav");
			m_movedJoystick = true;
		}
		else if(sf::Joystick::getAxisPosition(0, sf::Joystick::Y) == 100 &&
			getEngine()->getEvent().type == sf::Event::JoystickMoved)
		{
			++m_selectedItem;
			jl::SoundManager::playSound("res/menuSelect.wav");
			m_movedJoystick = true;
		}
	}

	if(sf::Joystick::getAxisPosition(0, sf::Joystick::Y) != -100 && sf::Joystick::getAxisPosition(0, sf::Joystick::Y) != 100)
		m_movedJoystick = false;


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
			int(getEngine()->getWindow().getSize().x*0.5) - (int)m_text.getGlobalBounds().width/2,
			int(getEngine()->getWindow().getSize().y*0.4) + i*int(m_text.getGlobalBounds().height*1.5));
		getEngine()->getWindow().draw(m_text);
	}

	// Draw title sprite
	m_titleSprite.setPosition(
		getEngine()->getWindow().getSize().x*0.5 - (m_titleSprite.getGlobalBounds().width/2), 
		getEngine()->getWindow().getSize().y*0.05);
	getEngine()->getWindow().draw(m_titleSprite);


	// Draw previous score
	m_scoreText.setString("Previous score: " + jl::Util::toString(jl::Settings::getInt("gameScore")));
	m_scoreText.setPosition(
		getEngine()->getWindow().getSize().x*0.05, 
		getEngine()->getWindow().getSize().y*0.9);
	getEngine()->getWindow().draw(m_scoreText);

	// Draw highscore
	m_scoreText.setString("Highscore: " + jl::Util::toString(jl::Settings::getInt("gameHighscore")));
	m_scoreText.setPosition(
		getEngine()->getWindow().getSize().x*0.95 - (m_scoreText.getGlobalBounds().width), 
		getEngine()->getWindow().getSize().y*0.9);
	getEngine()->getWindow().draw(m_scoreText);
}