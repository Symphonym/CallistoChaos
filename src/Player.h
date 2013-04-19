#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "TileCharacter.h"
#include "Weapon.h"

class GameState;
class Player : public TileCharacter
{
private:

	std::vector<std::shared_ptr<Weapon>> m_weapons;

	sf::Text m_resourceText, m_playerText;
	sf::Sprite m_healthSprite, m_ammoSprite, m_materialSprite, m_scoreSprite;

	// Selected weapon
	int m_selectedWeapon;
	// Player score
	int m_score;

public:

	explicit Player(GameState *gameState, jl::AssetManager &assets, const sf::Vector2i &tileIndex);

	virtual void events(sf::Event &events);
	virtual void update(double deltaTime);
	virtual void render(sf::RenderTarget &target);

	virtual void characterEvents(TileCharacter::Event events);

	void addWeapon(std::shared_ptr<Weapon> weapon);
	void addScore(int score);

	Weapon* getActiveWeapon();
	int getScore() const;
};

#endif