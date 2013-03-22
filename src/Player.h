#ifndef PLAYER_H
#define PLAYER_H

#include "TileCharacter.h"

class Player : public TileCharacter
{
private:

	void doWalkRight();
	void doWalkLeft();
	void doWalkUp();
	void doWalkDown();

public:

	explicit Player(TileMap &tilemap);

	void events(sf::Event &events);
	void update(double deltaTime);
	void render(sf::RenderTarget &target);
};

#endif