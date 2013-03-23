#ifndef PLAYER_H
#define PLAYER_H

#include "TileCharacter.h"

class Player : public TileCharacter
{
private:

	void duringWalkRight();
	void duringWalkLeft();
	void duringWalkUp();
	void duringWalkDown();

public:

	explicit Player(TileMap &tilemap, jl::AssetManager &assets, const sf::Vector2i &tileIndex);

	void events(sf::Event &events);
	void update(double deltaTime);
	void render(sf::RenderTarget &target);
};

#endif