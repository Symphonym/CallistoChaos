#include "RailGun.h"
#include "TileCharacter.h"
#include "TileMap.h"
#include "Utility.h"
#include "SoundManager.h"

RailGun::RailGun(const std::string &name, TileCharacter *tileCharacter, jl::AssetManager &assets) :
	Weapon(name, tileCharacter, assets)	
{
	
	setAmmo(10, 20);
	setFireRate(5);
	setBulletSpread(0);
	setBulletLifetime(2);
	setKnockBack(sf::Vector2f(10, 10));
	setCustomFire(true);
	setDamage(5);
	setFireSound("res/beamcharge.wav");
	jl::SoundManager::addSound("res/beamshoot.wav");
	setWeaponSheet(
		assets.getTexture("res/weapons.png"), 
		assets.getTexture("res/bullets.png"));

	addStance("right", sf::Vector2f(5, 3), sf::IntRect(0,32,16,16), sf::Vector2f(5, 4), sf::Vector2f(6, -1));
	addStance("left", sf::Vector2f(-5, 3), sf::IntRect(16,32,16,16), sf::Vector2f(5, 4), sf::Vector2f(-5, -1));
	addStance("up", sf::Vector2f(5, 5), sf::IntRect(32,32,16,16));
	addStance("down", sf::Vector2f(0, 5), sf::IntRect(48,32,16,16), sf::Vector2f(0.5,0));
	setStance("down");

	jl::FrameAnimation anim;
	anim.createAnimation("default");
	anim.pushFrame(sf::IntRect(0,48,16,16), 0.01).
		pushFrame(sf::IntRect(16,48,16,16), 0.01).
		pushFrame(sf::IntRect(32,48,16,16), 0.01).
		pushFrame(sf::IntRect(48,48,16,16), 0.01).
		pushFrame(sf::IntRect(64,48,16,16), 0.01).
		pushFrame(sf::IntRect(80,48,16,16), 0.01).
		pushFrame(sf::IntRect(96,48,16,16), 0.01).
		pushFrame(sf::IntRect(112,48,16,16), 0.01).
		pushFrame(sf::IntRect(128,48,16,16), 0.01).
		pushFrame(sf::IntRect(144,48,16,16), 0.01);
	addBulletAnimation("default");
	anim.createAnimation("bulletFire");
	anim.pushFrame(sf::IntRect(0,64,16,16), 0.01).
		pushFrame(sf::IntRect(16,64,16,16), 0.01).
		pushFrame(sf::IntRect(32,64,16,16), 0.01).
		pushFrame(sf::IntRect(48,64,16,16), 0.01).
		pushFrame(sf::IntRect(64,64,16,16), 0.01).
		pushFrame(sf::IntRect(80,64,16,16), 0.01).
		pushFrame(sf::IntRect(96,64,16,16), 0.01).
		pushFrame(sf::IntRect(112,64,16,16), 0.01).
		pushFrame(sf::IntRect(128,64,16,16), 0.01).
		pushFrame(sf::IntRect(144,64,16,16), 0.01).
		pushFrame(sf::IntRect(160,64,16,16), 0.01).
		pushFrame(sf::IntRect(176,64,16,16), 0.01).
		pushFrame(sf::IntRect(192,64,16,16), 0.01).
		pushFrame(sf::IntRect(208,64,16,16), 0.01);
	addBulletFireAnimation("bulletFire");

	setBulletAnimation(anim);
}

void RailGun::updateBullet(std::vector<Weapon::AnimatedSpriteData> &bullets, int i, double deltaTime)
{
	// Fadeout effect for beam
	sf::Color currentColor(bullets[i].sprite.getColor());
	currentColor.a = jl::Vec::lerp(sf::Vector2f(255, 0), sf::Vector2f(0, getBulletLifetime()), bullets[i].lifeTime/getBulletLifetime()).x;
	bullets[i].sprite.setColor(currentColor);

	sf::Vector2i index(getBulletIndex(bullets[i]));
	Tile *tile = &getTrackedChar().getTileMap().getTile(index);

	// Collision with solid tile, delete bullet
	if(tile->isSolid() && tile->isPlayerAttackable())
	{	
		tile->damage(calculateDamage(), &getTrackedChar().getTileMap(), index, bullets[i].baseDirection);
		bullets.erase(bullets.begin() + i);
	}
	// Collision with enemy, delete bullet
	else if (tile->isOccupied() && tile->isOccupied() && getTrackedChar().getIndex() != index)
	{
		if(bullets[i].sprite.getGlobalBounds().intersects(tile->getCharacter()->getSprite().getGlobalBounds()))
		{
			tile->damage(calculateDamage(), &getTrackedChar().getTileMap(), index, bullets[i].baseDirection);
		}
	}
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
		
	int indexCount = 0;

	// X-wise railgun
	if(gridDirection.x != 0)
	{
		int destinationX = 0;
		for(int x = getTrackedChar().getIndex().x+gridDirection.x; x < getTrackedChar().getTileMap().getMapSize().x && x >= 0; x+=gridDirection.x)
		{
			destinationX = x;

			// Break at collision, but allow the collision to take place(break after spawning)
			if(getLevel() < 4)
				if(getTrackedChar().getTileMap().getTile(x, getTrackedChar().getIndex().y).isSolid() &&
					getTrackedChar().getTileMap().getTile(x, getTrackedChar().getIndex().y).isPlayerAttackable())
					break;

		}

		int distanceX = gridDirection.x < 0 ? 
			std::abs((getTrackedChar().getIndex().x+gridDirection.x) - destinationX)+1 : 
			std::abs((getTrackedChar().getIndex().x+gridDirection.x) - destinationX)+1;

		for(int x = getTrackedChar().getIndex().x+gridDirection.x; x < getTrackedChar().getTileMap().getMapSize().x && x >= 0; x+=gridDirection.x)
		{
			++indexCount;

			sf::Vector2f tilePos(getTrackedChar().getTileMap().getTilePosition(x, getTrackedChar().getIndex().y));

			sf::Vector2f bulletPos(
				(tilePos.x + getTrackedChar().getTileMap().getTileSize()/2),
				tilePos.y + getTrackedChar().getTileMap().getTileSize()/2);

			bulletPos.y += getActiveStance().bulletPosition.y;

			spawnBullet(bulletPos, getBulletLifetime() - ((getBulletLifetime()/2)+((double)indexCount/(double)distanceX*getBulletLifetime()/2)));

			// Break at collision, but allow the collision to take place(break after spawning)
			if(getLevel() < 4)
				if(getTrackedChar().getTileMap().getTile(x, getTrackedChar().getIndex().y).isSolid() &&
					getTrackedChar().getTileMap().getTile(x, getTrackedChar().getIndex().y).isPlayerAttackable())
					break;
		}
	}

	// Y-wise railgun
	if(gridDirection.y != 0)
	{
		int destinationY = 0;
		for(int y = getTrackedChar().getIndex().y+gridDirection.y; y < getTrackedChar().getTileMap().getMapSize().y && y >= 0; y+=gridDirection.y)
		{
			destinationY = y;

			// Break at collision, but allow the collision to take place(break after spawning)
			if(getLevel() < 4)
				if(getTrackedChar().getTileMap().getTile(getTrackedChar().getIndex().x, y).isSolid() &&
					getTrackedChar().getTileMap().getTile(getTrackedChar().getIndex().x, y).isPlayerAttackable())
					break;
		}

		int distanceY = gridDirection.y < 0 ? 
			std::abs((getTrackedChar().getIndex().y+gridDirection.y) - destinationY)+1 : 
			std::abs((getTrackedChar().getIndex().y+gridDirection.y) - destinationY)+1;


		for(int y = getTrackedChar().getIndex().y+gridDirection.y; y < getTrackedChar().getTileMap().getMapSize().y && y >= 0; y+=gridDirection.y)
		{

			++indexCount;

			sf::Vector2f tilePos(getTrackedChar().getTileMap().getTilePosition(getTrackedChar().getIndex().x, y));

			sf::Vector2f bulletPos(
				tilePos.x + getTrackedChar().getTileMap().getTileSize()/2,
				tilePos.y + getTrackedChar().getTileMap().getTileSize()/2);

			bulletPos.x += getActiveStance().bulletPosition.x;

			spawnBullet(bulletPos, getBulletLifetime() - ((getBulletLifetime()/2)+((double)indexCount/(double)distanceY*getBulletLifetime()/2)));

			// Break at collision, but allow the collision to take place(break after spawning)
			if(getLevel() < 4)
				if(getTrackedChar().getTileMap().getTile(getTrackedChar().getIndex().x, y).isSolid() &&
					getTrackedChar().getTileMap().getTile(getTrackedChar().getIndex().x, y).isPlayerAttackable())
					break;
		}
	}
}

double RailGun::calculateFireRate() const
{
	return 7 - (getLevel()-1);
}
int RailGun::calculateMaxAmmo() const
{
	return 10 + getLevel()*10;
}
