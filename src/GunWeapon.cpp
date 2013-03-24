#include "GunWeapon.h"

GunWeapon::GunWeapon(TileCharacter *tileCharacter, jl::AssetManager &assets) :
	Weapon(tileCharacter, assets)
{
	setAmmoCost(0);
	setFireRate(0.5);
	setBulletSpeed(100);
	setWeaponSheet(
		assets.getAsset<jl::TextureAsset>("res/weapons.png")->get(),
		assets.getAsset<jl::TextureAsset>("res/bullets.png")->get());

	addStance("right", sf::Vector2f(0,0), sf::IntRect(0,0,16,16));
	setStance("right");

	m_bulletSprite.setTextureRect(sf::IntRect(0,0,16,16));

}

void GunWeapon::update(double deltaTime)
{
	for(std::size_t i = 0; i < m_bullets.size(); i++)
	{
		switch(m_bullets[i].second)
		{
			case Weapon::Right:
				m_bullets[i].first.x += m_bulletSpeed*deltaTime;
			break;
			case Weapon::Left:
				m_bullets[i].first.y -= m_bulletSpeed*deltaTime;
			break;
			case Weapon::Up:
				m_bullets[i].first.y -= m_bulletSpeed*deltaTime;
			break;
			case Weapon::Down:
				m_bullets[i].first.y += m_bulletSpeed*deltaTime;
			break;
		}
	}
}
void GunWeapon::render(sf::RenderTarget &target)
{
	// Keep in mind that origin is at center
	float xPos = m_trackedCharacter->getSprite().getPosition().x +
	(m_stances[m_activeStance].position.x + m_stances[m_activeStance].subRect.width/2);
	float yPos = m_trackedCharacter->getSprite().getPosition().y +
	(m_stances[m_activeStance].position.y + m_stances[m_activeStance].subRect.height/2);

	m_weaponSprite.setPosition(xPos, yPos);
	target.draw(m_weaponSprite);

	for(std::size_t i = 0; i < m_bullets.size(); i++)
	{
		m_bulletSprite.setPosition(m_bullets[i].first);
		target.draw(m_bulletSprite);
	}
}