#ifndef RIFLEWEAPON_H
#define RIFLEWEAPON_H

#include <SFML/Graphics.hpp>
#include "Weapon.h"

class RifleWeapon : public Weapon
{

public:
	explicit RifleWeapon(const std::string &name, TileCharacter *tileCharacter, jl::AssetManager &assets);
};

#endif