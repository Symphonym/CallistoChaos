#include "GameState.h"
#include <iostream>
#include "SoundManager.h"

GameState::GameState(jl::Engine *engine) : jl::State(engine)
{
}

void Game::init()
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
	getEngine()->m_assets.getAsset<jl::SoundBufferAsset>("button.wav");
	jl::SoundManager::addSound("button.wav");

	sprite.setTexture(getEngine()->m_assets.getAsset<jl::TextureAsset>("rpgmaker.png")->get());
	sprite.setPosition(0,0);
	view = getEngine()->createView(0.2);
}
void GameState::events()
{

}
void GameState::update()
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		anim.request("up");
		sprite.move(0, -100*getEngine()->getDelta());
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		anim.request("down");
		sprite.move(0, 100*getEngine()->getDelta());
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		anim.request("left");
		sprite.move(-100*getEngine()->getDelta(), 0);
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		anim.request("right");
		sprite.move(100*getEngine()->getDelta(), 0);
	}

	anim.commit(sprite, getEngine()->getDelta());

	if(sprite.getPosition().x > (view.getSize().x - sprite.getGlobalBounds().width))
		sprite.setPosition((view.getSize().x - sprite.getGlobalBounds().width), sprite.getPosition().y);
	if(sprite.getPosition().y < 0)
		sprite.setPosition(sprite.getPosition().x, 0);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		view.zoom(1.0 - getEngine()->getDelta());
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		view.zoom(1.0 + getEngine()->getDelta());

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		view.rotate(100*getEngine()->getDelta());

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::T))
		getEngine()->m_stack.popState();

	getEngine()->m_window.setView(view);
}
void GameState::render()
{
	getEngine()->render(sprite);
}