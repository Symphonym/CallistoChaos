#include "RifleWeapon.h"
#include "TileMap.h"

RifleWeapon::RifleWeapon(const std::string &name, TileCharacter *tileCharacter, jl::AssetManager &assets) :
	Weapon(name, tileCharacter, assets)
{
	setBulletSpeed(250);
	setKnockBack(sf::Vector2f(5, 5));
	setWeaponSheet(
		assets.getTexture("res/weapons.png"),
		assets.getTexture("res/bullets.png"));
	setFireSound("res/lasershoot.wav");

	addStance("right", sf::Vector2f(5,5), sf::IntRect(0,16,16,16), sf::Vector2f(6, -2.7), sf::Vector2f(7, -1.5));
	addStance("left", sf::Vector2f(-5, 5), sf::IntRect(16,16,16,16), sf::Vector2f(-6, -2.7), sf::Vector2f(-7.5, -1.5));
	addStance("up", sf::Vector2f(5, 5), sf::IntRect(32,16,16,16));
	addStance("down", sf::Vector2f(0, 5), sf::IntRect(48,16,16,16), sf::Vector2f(0.5,0));
	setStance("down");

	jl::FrameAnimation anim;
	anim.createAnimation("default");
	anim.pushFrame(sf::IntRect(0,0,16,16), 0.05).
		pushFrame(sf::IntRect(16,0,16,16), 0.05).
		pushFrame(sf::IntRect(32,0,16,16), 0.05).
		pushFrame(sf::IntRect(48,0,16,16), 0.05);
	addBulletAnimation("default");
	anim.createAnimation("bulletFire");
	anim.pushFrame(sf::IntRect(0,32,16,16), 0.01).
		pushFrame(sf::IntRect(16,32,16,16), 0.01).
		pushFrame(sf::IntRect(32,32,16,16), 0.01).
		pushFrame(sf::IntRect(48,32,16,16), 0.01).
		pushFrame(sf::IntRect(64,32,16,16), 0.01).
		pushFrame(sf::IntRect(80,32,16,16), 0.01).
		pushFrame(sf::IntRect(96,32,16,16), 0.01);
	addBulletFireAnimation("bulletFire");

	setBulletAnimation(anim);
}

int RifleWeapon::calculateDamage() const
{
	if(getLevel() >= 5)
		return 2;
	return 1;
}
int RifleWeapon::calculateCost() const
{
	if(getLevel() >= 5)
		return 2;
	return 1;
}
int RifleWeapon::calculateMaxAmmo() const
{
	return 40+(10*getLevel());
}
double RifleWeapon::calculateFireRate() const
{
	return 0.1 - (0.005*getLevel());
}
double RifleWeapon::calculateBulletSpread() const
{
	return 10 - (0.5*getLevel());
}