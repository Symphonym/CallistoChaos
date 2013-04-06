#ifndef WEAKENEMY_H
#define WEAKENEMY_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "TileCharacter.h"

class GameState;
class WeakEnemy : public TileCharacter
{
private:

	sf::Clock m_moveTimer;
	double m_moveDelay;

	int m_damage;

public:

	explicit WeakEnemy(GameState *gameState, jl::AssetManager &assets, const sf::Vector2i &tileIndex);

	virtual void events(sf::Event &events);
	virtual void update(double deltaTime);
	virtual void render(sf::RenderTarget &target);

	virtual void characterEvents(TileCharacter::Event events);
};

#endif