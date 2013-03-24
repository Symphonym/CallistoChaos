#ifndef GUNWEAPON_H
#define GUNWEAPON_H

#include <SFML/Graphics.hpp>
#include "Weapon.h"

class GunWeapon : public Weapon
{

public:
	explicit GunWeapon(TileCharacter *tileCharacter, jl::AssetManager &assets);

	void update(double deltaTime);
	void render(sf::RenderTarget &target);
};

#endif