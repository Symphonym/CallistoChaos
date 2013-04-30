#include "WeakEnemy.h"
#include "AstarAlgorithm.h"
#include "GameState.h"
#include "ParticleManager.h"
#include "Utility.h"

WeakEnemy::WeakEnemy(GameState *gameState, jl::AssetManager &assets, const sf::Vector2i &tileIndex) :
	TileCharacter(gameState, assets, tileIndex),
	m_moveDelay(0.1),
	m_damage(1)
{
	setMaxHealth(5);
	heal(5);

	m_animation.createAnimation("right");
	m_animation.pushFrame(sf::IntRect(64, 48, 16, 16), 0.05).
		pushFrame(sf::IntRect(80, 48, 16, 16), 0.05).
		pushFrame(sf::IntRect(96, 48, 16, 16), 0.05).
		pushFrame(sf::IntRect(112, 48, 16, 16), 0.05);
	m_animation.createAnimation("left");
	m_animation.pushFrame(sf::IntRect(64, 32, 16, 16), 0.05).
		pushFrame(sf::IntRect(80, 32, 16, 16), 0.05).
		pushFrame(sf::IntRect(96, 32, 16, 16), 0.05).
		pushFrame(sf::IntRect(112, 32, 16, 16), 0.05);
	m_animation.createAnimation("up");
	m_animation.pushFrame(sf::IntRect(64, 16, 16, 16), 0.05).
		pushFrame(sf::IntRect(80, 16, 16, 16), 0.05).
		pushFrame(sf::IntRect(96, 16, 16, 16), 0.05).
		pushFrame(sf::IntRect(112, 16, 16, 16), 0.05);
	m_animation.createAnimation("down");
	m_animation.pushFrame(sf::IntRect(64, 0, 16, 16), 0.05).
		pushFrame(sf::IntRect(80, 0, 16, 16), 0.05).
		pushFrame(sf::IntRect(96, 0, 16, 16), 0.05).
		pushFrame(sf::IntRect(112, 0, 16, 16), 0.05);
	m_animation.createAnimation("lookRight");
	m_animation.pushFrame(sf::IntRect(64, 48, 16, 16), 0.05).
		pushFrame(sf::IntRect(80, 48, 16, 16), 0.05).
		pushFrame(sf::IntRect(96, 48, 16, 16), 0.05).
		pushFrame(sf::IntRect(112, 48, 16, 16), 0.05);
	m_animation.createAnimation("lookLeft");
	m_animation.pushFrame(sf::IntRect(64, 32, 16, 16), 0.05).
		pushFrame(sf::IntRect(80, 32, 16, 16), 0.05).
		pushFrame(sf::IntRect(96, 32, 16, 16), 0.05).
		pushFrame(sf::IntRect(112, 32, 16, 16), 0.05);
	m_animation.createAnimation("lookUp");
	m_animation.pushFrame(sf::IntRect(64, 16, 16, 16), 0.05).
		pushFrame(sf::IntRect(80, 16, 16, 16), 0.05).
		pushFrame(sf::IntRect(96, 16, 16, 16), 0.05).
		pushFrame(sf::IntRect(112, 16, 16, 16), 0.05);
	m_animation.createAnimation("lookDown");
	m_animation.pushFrame(sf::IntRect(64, 0, 16, 16), 0.05).
		pushFrame(sf::IntRect(80, 0, 16, 16), 0.05).
		pushFrame(sf::IntRect(96, 0, 16, 16), 0.05).
		pushFrame(sf::IntRect(112, 0, 16, 16), 0.05);

	m_animation.initAnimation(m_sprite, "down");
	m_sprite.setTexture(assets.getTexture("res/rpgmaker16.png"));
}


void WeakEnemy::setMoveDelay(double moveDelay)
{
	m_moveDelay = moveDelay;
}

void WeakEnemy::events(sf::Event &events)
{

}

void WeakEnemy::update(double deltaTime)
{
	if(m_moveTimer.getElapsedTime().asSeconds() >= m_moveDelay && !isDead())
	{
		m_moveTimer.restart();

		sf::Vector2i pathGoal = getChars().getPlayer().isDead() ? 
			sf::Vector2i(
				std::rand() % m_gameState->getTileMap().getMapSize().x,
				std::rand() % m_gameState->getTileMap().getMapSize().y) : getChars().getPlayer().getIndex();

		bool disregardEnemies = getGame().getChars().getCount() < CharacterManager::characterLimit - 2 ? false : true;
		std::vector<AstarNode> path = AstarAlgorithm::findPath(getIndex(), pathGoal, &getTileMap(), disregardEnemies);

		sf::Vector2i pathIndex = path[0].index;

		// Right of enemy
		if(pathIndex == sf::Vector2i(getIndex().x + 1, getIndex().y))
			walkRight();
		// Left of enemy
		else if(pathIndex == sf::Vector2i(getIndex().x - 1, getIndex().y))
			walkLeft();
		// Above enemy
		else if(pathIndex == sf::Vector2i(getIndex().x, getIndex().y - 1))
			walkUp();
		// Below enemy
		else if(pathIndex == sf::Vector2i(getIndex().x, getIndex().y + 1))
			walkDown();

		// Attack tile
		if(!isWalking())
		{
			if(m_gameState->getTileMap().getTile(pathIndex).getTileType() == 5 ||
				m_gameState->getTileMap().getTile(pathIndex).getTileType() == 6 ||
				(m_gameState->getTileMap().getTile(pathIndex).isOccupied() && dynamic_cast<Player*>(m_gameState->getTileMap().getTile(pathIndex).getCharacter())))
			{
				m_gameState->getTileMap().getTile(pathIndex).damage(m_damage, &m_gameState->getTileMap(), pathIndex);
			}
		}

	}

		m_animation.commit(m_sprite, deltaTime);
}
void WeakEnemy::render(sf::RenderTarget &target)
{
	target.draw(m_sprite);
}

void WeakEnemy::characterEvents(TileCharacter::Event events)
{
	if(events == TileCharacter::WalkingRight)
		m_animation.request("right");
	else if(events == TileCharacter::WalkingLeft)
		m_animation.request("left");
	else if(events == TileCharacter::WalkingUp)
		m_animation.request("up");
	else if(events == TileCharacter::WalkingDown)
		m_animation.request("down");

	else if(events == TileCharacter::LookingRight)
		m_animation.request("lookRight");
	else if(events == TileCharacter::LookingLeft)
		m_animation.request("lookLeft");
	else if(events == TileCharacter::LookingUp)
		m_animation.request("lookUp");
	else if(events == TileCharacter::LookingDown)
		m_animation.request("lookDown");
}
