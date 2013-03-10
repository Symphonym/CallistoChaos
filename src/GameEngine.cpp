#include "GameEngine.h"
#include "Locator.h"

namespace cc
{
	GameEngine::GameEngine()
	{
		sf::Image image;
		image.create(50, 50, sf::Color::Green);
		texture.loadFromImage(image);

		sprite.setTexture(texture);
		sprite.setPosition(0,0);

		view = Locator::getRender()->createView(0.5);

		Locator::getRender()->setView(view);
	}

	void GameEngine::events()
	{

	}
	void GameEngine::update()
	{
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			sprite.move(0, -100*getDelta());
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			sprite.move(0, 100*getDelta());
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			sprite.move(-100*getDelta(), 0);
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			sprite.move(100*getDelta(), 0);

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			view.zoom(1.0 - getDelta());
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
			view.zoom(1.0 + getDelta());

		Locator::getRender()->setView(view);
	}
	void GameEngine::render()
	{
		Locator::getRender()->render(sprite);
	}
};