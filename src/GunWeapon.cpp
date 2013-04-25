#include "GunWeapon.h"
#include "TileMap.h"

GunWeapon::GunWeapon(const std::string &name, TileCharacter *tileCharacter, jl::AssetManager &assets) :
	Weapon(name, tileCharacter, assets)
{
	setAmmo(10, -1);
	setFireRate(0.5);
	setBulletSpread(5);
	setKnockBack(sf::Vector2f(5, 5));
	setFireSound("res/lasershoot.wav");
	setWeaponSheet(
		assets.getTexture("res/weapons.png"),
		assets.getTexture("res/bullets.png"));

	addStance("right", sf::Vector2f(5,5), sf::IntRect(0,0,16,16), sf::Vector2f(6, -2.7), sf::Vector2f(6, -1.5));
	addStance("left", sf::Vector2f(-5, 5), sf::IntRect(16,0,16,16), sf::Vector2f(-7, -2.7), sf::Vector2f(-4.5, 0));
	addStance("up", sf::Vector2f(5, 5), sf::IntRect(32,0,16,16));
	addStance("down", sf::Vector2f(0, 5), sf::IntRect(48,0,16,16), sf::Vector2f(0.5,0));
	setStance("down");

	jl::FrameAnimation anim;
	anim.createAnimation("default");
	anim.pushFrame(sf::IntRect(0,0,16,16), 0.1).
		pushFrame(sf::IntRect(16,0,16,16), 0.1).
		pushFrame(sf::IntRect(32,0,16,16), 0.1).
		pushFrame(sf::IntRect(48,0,16,16), 0.1);
	addBulletAnimation("default");
	anim.createAnimation("bulletFire");
	anim.pushFrame(sf::IntRect(0,32,16,16), 0.01).
		pushFrame(sf::IntRect(16,32,16,16), 0.01).
		pushFrame(sf::IntRect(32,32,16,16), 0.01).
		pushFrame(sf::IntRect(48,32,16,16), 0.01).
		pushFrame(sf::IntRect(64,32,16,16), 0.01).
		pushFrame(sf::IntRect(80,32,16,16), 0.01);
	addBulletFireAnimation("bulletFire");

	setBulletAnimation(anim);

}
int GunWeapon::calculateDamage() const
{
	return std::ceil(getLevel()/2.0);
}
double GunWeapon::calculateSpeed() const
{
	return 100 + (50*getLevel());
}
