#include "GameEngine.h"
#include "EngineInfo.h"

GameEngine::GameEngine()
{
}

void GameEngine::init()
{
	cc::EngineInfo::getAssets().addAsset<cc::TextureAsset>("wow.png");

	sprite.setTexture(cc::EngineInfo::acquire<cc::TextureAsset>("wow.png")->get());
	sprite.setPosition(0,0);
	view = cc::EngineInfo::getRender().createView(0.5);
}
void GameEngine::events()
{

}
void GameEngine::update()
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		sprite.move(0, -100*cc::EngineInfo::getDelta());
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		sprite.move(0, 100*cc::EngineInfo::getDelta());
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		sprite.move(-100*cc::EngineInfo::getDelta(), 0);
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		sprite.move(100*cc::EngineInfo::getDelta(), 0);

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