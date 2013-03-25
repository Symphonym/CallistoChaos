#ifndef GUNWEAPON_H
#define GUNWEAPON_H

#include <SFML/Graphics.hpp>
#include "Weapon.h"

class GunWeapon : public Weapon
{

public:
	explicit GunWeapon(const std::string &name, TileCharacter *tileCharacter, jl::AssetManager &assets);
};

#endif