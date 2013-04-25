#include "RailGun.h"
#include "TileCharacter.h"
#include "TileMap.h"
#include "Utility.h"
#include "SoundManager.h"

RailGun::RailGun(const std::string &name, TileCharacter *tileCharacter, jl::AssetManager &assets) :
	Weapon(name, tileCharacter, assets)	
{
	
	setAmmo(5, -1);
	setFireRate(0.5);
	setBulletSpread(0);
	setBulletLifetime(5);
	setKnockBack(sf::Vector2f(10, 10));
	setCustomFire(true);
	setFireSound("res/beamcharge.wav");
	jl::SoundManager::addSound("res/beamshoot.wav");
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

void RailGun::updateBullet(AnimatedSpriteData &bullet, double deltaTime)
{
	// Fadeout effect for beam
	sf::Color currentColor(bullet.sprite.getColor());
	currentColor.a = jl::Vec::lerp(sf::Vector2f(255, 0), sf::Vector2f(0, getBulletLifetime()), bullet.lifeTime/getBulletLifetime()).x;
	bullet.sprite.setColor(currentColor);
}
void RailGun::bulletFireCallBack()
{
	jl::SoundManager::playSound("res/beamshoot.wav");

	sf::Vector2i gridDirection(0,0);

	if(getTrackedChar().lookingRight())
		gridDirection.x = 1;
	else if(getTrackedChar().lookingLeft())
		gridDirection.x = -1;
	else if(getTrackedChar().lookingUp())
		gridDirection.y = -1;
	else if(getTrackedChar().lookingDown())
		gridDirection.y = 1;
		
	// X-wise railgun
	for(int x = getTrackedChar().getIndex().x+gridDirection.x; x < getTrackedChar().getTileMap().getMapSize().x && x >= 0; x+=gridDirection.x)
	{
		sf::Vector2f tilePos(getTrackedChar().getTileMap().getTilePosition(x, getTrackedChar().getIndex().y));

		sf::Vector2f bulletPos(
			tilePos.x + getTrackedChar().getTileMap().getTileSize()/2,
			tilePos.y + getTrackedChar().getTileMap().getTileSize()/2);

		spawnBullet(bulletPos);

		// Break at collision, but allow the collision to take place(break after spawning)
		if(getLevel() < 4)
			if(getTrackedChar().getTileMap().getTile(x, getTrackedChar().getIndex().y).isSolid() ||
				getTrackedChar().getTileMap().getTile(x, getTrackedChar().getIndex().y).isOccupied())
				break;
	}


	for(int y = getTrackedChar().getIndex().y+gridDirection.y; y < getTrackedChar().getTileMap().getMapSize().y && y >= 0; y+=gridDirection.y)
	{
		sf::Vector2f tilePos(getTrackedChar().getTileMap().getTilePosition(getTrackedChar().getIndex().x, y));

		sf::Vector2f bulletPos(
			tilePos.x + getTrackedChar().getTileMap().getTileSize()/2,
			tilePos.y + getTrackedChar().getTileMap().getTileSize()/2);

		spawnBullet(bulletPos);

		// Break at collision, but allow the collision to take place(break after spawning)
		if(getLevel() < 4)
			if(getTrackedChar().getTileMap().getTile(getTrackedChar().getIndex().x, y).isSolid() ||
				getTrackedChar().getTileMap().getTile(getTrackedChar().getIndex().x, y).isOccupied())
				break;
	}
}

int RailGun::calculateDamage() const
{
	return 5;
}
