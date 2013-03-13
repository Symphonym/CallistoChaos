#include "GameEngine.h"
#include "EngineInfo.h"
#include <iostream>

GameEngine::GameEngine()
{
}

void GameEngine::init()
{
	const double duration = 0.1;
	cc::EngineInfo::getAssets().addAsset<cc::TextureAsset>("rpgmaker.png");
	anim.createAnimation("right");
	anim.pushFrame(sf::IntRect(0, 64, 32, 32), duration).
		 pushFrame(sf::IntRect(32, 64, 32, 32), duration).
		 pushFrame(sf::IntRect(64, 64, 32, 32), duration);
	anim.createAnimation("left");
	anim.pushFrame(sf::IntRect(0, 32, 32, 32), duration).
		 pushFrame(sf::IntRect(32, 32, 32, 32), duration).
		 pushFrame(sf::IntRect(64, 32, 32, 32), duration);
	anim.createAnimation("up");
	anim.pushFrame(sf::IntRect(0, 96, 32, 32), duration).
		 pushFrame(sf::IntRect(32, 96, 32, 32), duration).
		 pushFrame(sf::IntRect(64, 96, 32, 32), duration);
	anim.createAnimation("down");
	anim.pushFrame(sf::IntRect(0, 0, 32, 32), duration).
		 pushFrame(sf::IntRect(32, 0, 32, 32), duration).
		 pushFrame(sf::IntRect(64, 0, 32, 32), duration);
	anim.initAnimation(sprite, "down");



	sprite.setTexture(cc::EngineInfo::acquire<cc::TextureAsset>("rpgmaker.png")->get());
	sprite.setPosition(0,0);
	view = cc::EngineInfo::getRender().createView(0.5);
}
void GameEngine::events()
{

}
void GameEngine::update()
{
	std::string animstring = "";
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		//animstring = "up";
		anim.request("up");
		sprite.move(0, -100*cc::EngineInfo::getDelta());
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		//animstring = "down";
		anim.request("down");
		sprite.move(0, 100*cc::EngineInfo::getDelta());
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		//animstring = "left";
		anim.request("left");
		sprite.move(-100*cc::EngineInfo::getDelta(), 0);
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		//animstring = "right";
		anim.request("right");
		sprite.move(100*cc::EngineInfo::getDelta(), 0);
	}

	//anim.animate(sprite, animstring);
	anim.commit(sprite);

	if(sprite.getPosition().x > (view.getSize().x - sprite.getGlobalBounds().width))
		sprite.setPosition((view.getSize().x - sprite.getGlobalBounds().width), sprite.getPosition().y);
	if(sprite.getPosition().y < 0)
		sprite.setPosition(sprite.getPosition().x, 0);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		view.zoom(1.0 - cc::EngineInfo::getDelta());
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		view.zoom(1.0 + cc::EngineInfo::getDelta());

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		view.rotate(100*cc::EngineInfo::getDelta());

	cc::EngineInfo::getRender().setView(view);
}
void GameEngine::render()
{
	cc::EngineInfo::render(sprite);
}