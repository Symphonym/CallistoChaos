#include "GameEngine.h"
#include <iostream>
#include "SoundManager.h"

GameEngine::GameEngine(jl::Engine *engine) : jl::State(engine)
{
}

void GameEngine::init()
{
	const double duration = 0.1;
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
	anim.pushFrame(sf::IntRect(0, 0, 32, 32), duration, "button.wav").
		 pushFrame(sf::IntRect(32, 0, 32, 32), duration).
		 pushFrame(sf::IntRect(64, 0, 32, 32), duration);
	anim.initAnimation(sprite, "down");

	// Load sound
	getEngine()->getAssets().getAsset<jl::SoundBufferAsset>("button.wav");
	jl::SoundManager::addSound("button.wav");

	sprite.setTexture(getEngine()->getAssets().getAsset<jl::TextureAsset>("rpgmaker.png")->get());
	sprite.setPosition(0,0);
	view = getEngine()->createView(0.2);
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
		sprite.move(0, -100*getDelta());
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		//animstring = "down";
		anim.request("down");
		sprite.move(0, 100*getDelta());
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		//animstring = "left";
		anim.request("left");
		sprite.move(-100*getDelta(), 0);
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		//animstring = "right";
		anim.request("right");
		sprite.move(100*getDelta(), 0);
	}

	//anim.animate(sprite, animstring);
	anim.commit(sprite, getDelta());

	if(sprite.getPosition().x > (view.getSize().x - sprite.getGlobalBounds().width))
		sprite.setPosition((view.getSize().x - sprite.getGlobalBounds().width), sprite.getPosition().y);
	if(sprite.getPosition().y < 0)
		sprite.setPosition(sprite.getPosition().x, 0);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		view.zoom(1.0 - getDelta());
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		view.zoom(1.0 + getDelta());

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		view.rotate(100*getDelta());

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::T))
		getEngine()->getStack().popState();

	getEngine()->getWindow().setView(view);
}
void GameEngine::render()
{
	getEngine()->getWindow().draw(sprite);
}