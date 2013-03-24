#ifndef GUNWEAPON_H
#define GUNWEAPON_H

#include <SFML/Graphics.hpp>
#include "Weapon.h"

class GunWeapon : public Weapon
{

public:
	explicit GunWeapon(const std::string &name, TileCharacter *tileCharacter, jl::AssetManager &assets);

	virtual void update(std::size_t index, double deltaTime);
	virtual void render(std::size_t index, sf::RenderTarget &target);
};

#endif