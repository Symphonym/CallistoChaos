#include "GameEngine.h"
#include "EngineInfo.h"

namespace cc
{
	GameEngine::GameEngine()
	{
		sf::Image image;
		image.create(50, 50, sf::Color::Green);
		texture.loadFromImage(image);

		sprite.setTexture(texture);
		sprite.setPosition(0,0);
	}

	void GameEngine::init()
	{
		view = EngineInfo::getRender().createView(0.5);
	}
	void GameEngine::events()
	{

	}
	void GameEngine::update()
	{
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			sprite.move(0, -100*EngineInfo::getDelta());
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			sprite.move(0, 100*EngineInfo::getDelta());
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			sprite.move(-100*EngineInfo::getDelta(), 0);
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			sprite.move(100*EngineInfo::getDelta(), 0);

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			view.zoom(1.0 - EngineInfo::getDelta());
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
			view.zoom(1.0 + EngineInfo::getDelta());

		EngineInfo::getRender().setView(view);
	}
	void GameEngine::render()
	{
		EngineInfo::getRender().render(sprite);
	}
};