#ifndef RAILGUN_H
#define RAILGUN_H

#include <SFML/Graphics.hpp>
#include "Weapon.h"

class RailGun : public Weapon
{

public:
	explicit RailGun(const std::string &name, TileCharacter *tileCharacter, jl::AssetManager &assets);

	virtual void updateBullet(AnimatedSpriteData &bullet, double deltaTime);
	virtual void bulletFireCallBack();

	int calculateDamage() const;
};

#endif
