#include "GunWeapon.h"
#include "TileMap.h"

GunWeapon::GunWeapon(const std::string &name, TileCharacter *tileCharacter, jl::AssetManager &assets) :
	Weapon(name, tileCharacter, assets)
{
	setAmmoCost(10);
	setFireRate(0.5);
	setBulletSpeed(200);
	setWeaponSheet(
		assets.getAsset<jl::TextureAsset>("res/weapons.png")->get(),
		assets.getAsset<jl::TextureAsset>("res/bullets.png")->get());

	addStance("right", sf::Vector2f(5,5), sf::IntRect(0,0,16,16));
	addStance("left", sf::Vector2f(-5, 5), sf::IntRect(16,0,16,16));
	addStance("up", sf::Vector2f(5, 5), sf::IntRect(32,0,16,16));
	addStance("down", sf::Vector2f(0, 5), sf::IntRect(48,0,16,16), sf::Vector2f(0.5,0));
	setStance("down");

	jl::FrameAnimation anim;
	anim.createAnimation("default");
	anim.pushFrame(sf::IntRect(0,0,16,16), 0.05).
		pushFrame(sf::IntRect(16,0,16,16), 0.05).
		pushFrame(sf::IntRect(32,0,16,16), 0.05).
		pushFrame(sf::IntRect(48,0,16,16), 0.05);
	setBulletAnimation(anim);


}
void GunWeapon::update(std::size_t i, double deltaTime)
{	
	sf::Vector2i index(getBulletIndex(m_bullets[i]));
	switch(m_bullets[i].direction)
	{
		case Weapon::Right:
			m_bullets[i].sprite.move(m_bulletSpeed*deltaTime, 0);
		break;
		case Weapon::Left:
			m_bullets[i].sprite.move(-m_bulletSpeed*deltaTime, 0);
		break;
		case Weapon::Up:
			m_bullets[i].sprite.move(0, -m_bulletSpeed*deltaTime);
		break;
		case Weapon::Down:
			m_bullets[i].sprite.move(0, m_bulletSpeed*deltaTime);
		break;
	}

	Tile *tile = &m_trackedCharacter->getTileMap().getTile(index);

	// Collision with solid tile
	if(tile->isSolid() && tile->isPlayerAttackable())
	{	
		tile->damage(1);
		m_bullets.erase(m_bullets.begin() + i);
		return;
	}


	m_bullets[i].animation.animate(m_bullets[i].sprite, "default", deltaTime);
}
void GunWeapon::render(std::size_t i, sf::RenderTarget &target)
{
	target.draw(m_bullets[i].sprite);
}