#include "RifleWeapon.h"
#include "TileMap.h"

RifleWeapon::RifleWeapon(const std::string &name, TileCharacter *tileCharacter, jl::AssetManager &assets) :
	Weapon(name, tileCharacter, assets)
{
	setAmmo(1, 50);
	setFireRate(0.1);
	setBulletSpeed(250);
	setWeaponSheet(
		assets.getAsset<jl::TextureAsset>("res/weapons.png")->get(),
		assets.getAsset<jl::TextureAsset>("res/bullets.png")->get());

	addStance("right", sf::Vector2f(5,5), sf::IntRect(0,16,16,16), sf::Vector2f(6, -2.7));
	addStance("left", sf::Vector2f(-5, 5), sf::IntRect(16,16,16,16), sf::Vector2f(-6, -2.7));
	addStance("up", sf::Vector2f(5, 5), sf::IntRect(32,16,16,16));
	addStance("down", sf::Vector2f(0, 5), sf::IntRect(48,16,16,16), sf::Vector2f(0.5,0));
	setStance("down");

	jl::FrameAnimation anim;
	anim.createAnimation("default");
	anim.pushFrame(sf::IntRect(0,0,16,16), 0.05).
		pushFrame(sf::IntRect(16,0,16,16), 0.05).
		pushFrame(sf::IntRect(32,0,16,16), 0.05).
		pushFrame(sf::IntRect(48,0,16,16), 0.05);
	setBulletAnimation(anim);
}

void RifleWeapon::update(BulletData &bullet, double deltaTime)
{	
	sf::Vector2i index(getBulletIndex(bullet));
	switch(bullet.direction)
	{
		case Weapon::Right:
			bullet.sprite.move(getSpeed(deltaTime), 0);
		break;
		case Weapon::Left:
			bullet.sprite.move(-getSpeed(deltaTime), 0);
		break;
		case Weapon::Up:
			bullet.sprite.move(0, -getSpeed(deltaTime));
		break;
		case Weapon::Down:
			bullet.sprite.move(0, getSpeed(deltaTime));
		break;
	}

	bullet.animation.animate(bullet.sprite, "default", deltaTime);
}
void RifleWeapon::render(BulletData &bullet, sf::RenderTarget &target)
{
	target.draw(bullet.sprite);
}