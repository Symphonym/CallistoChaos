#include "GameEngine.h"
#include "EngineInfo.h"

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
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		anim.animate(sprite, "up");
		sprite.move(0, -100*cc::EngineInfo::getDelta());
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		anim.animate(sprite, "down");
		sprite.move(0, 100*cc::EngineInfo::getDelta());
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		anim.animate(sprite, "left");
		sprite.move(-100*cc::EngineInfo::getDelta(), 0);
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		anim.animate(sprite, "right");
		sprite.move(100*cc::EngineInfo::getDelta(), 0);
	}

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