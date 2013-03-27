#include "RifleWeapon.h"
#include "TileMap.h"

RifleWeapon::RifleWeapon(const std::string &name, TileCharacter *tileCharacter, jl::AssetManager &assets) :
	Weapon(name, tileCharacter, assets)
{
	setAmmo(1, -1);
	setFireRate(0.1);
	setDamage(10);
	setBulletSpeed(250);
	setBulletSpread(10);
	setKnockBack(sf::Vector2f(5, 5));
	setWeaponSheet(
		assets.getAsset<jl::TextureAsset>("res/weapons.png")->get(),
		assets.getAsset<jl::TextureAsset>("res/bullets.png")->get());

	addStance("right", sf::Vector2f(5,5), sf::IntRect(0,16,16,16), sf::Vector2f(6, -2.7), sf::Vector2f(7, -1.5));
	addStance("left", sf::Vector2f(-5, 5), sf::IntRect(16,16,16,16), sf::Vector2f(-6, -2.7), sf::Vector2f(-7, -1.5));
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
	anim.pushFrame(sf::IntRect(0,32,16,16), 0.0001).
		pushFrame(sf::IntRect(16,32,16,16), 0.0001).
		pushFrame(sf::IntRect(32,32,16,16), 0.0001).
		pushFrame(sf::IntRect(48,32,16,16), 0.0001).
		pushFrame(sf::IntRect(64,32,16,16), 0.0001).
		pushFrame(sf::IntRect(80,32,16,16), 0.0001).
		pushFrame(sf::IntRect(96,32,16,16), 0.0001);
	addBulletFireAnimation("bulletFire");

	setBulletAnimation(anim);
}