#include "WeakEnemy.h"
#include "AstarAlgorithm.h"
#include "GameState.h"

WeakEnemy::WeakEnemy(GameState *gameState, jl::AssetManager &assets, const sf::Vector2i &tileIndex) :
	TileCharacter(gameState, assets, tileIndex),
	m_moveDelay(0.1),
	m_damage(1)
{
	setSpeed(50);
	setMaxHealth(5);

	m_animation.createAnimation("right");
	m_animation.pushFrame(sf::IntRect(0, 32, 16, 16), 0.1).
		pushFrame(sf::IntRect(16, 32, 16, 16), 0.1).
		pushFrame(sf::IntRect(32, 32, 16, 16), 0.1);
	m_animation.createAnimation("left");
	m_animation.pushFrame(sf::IntRect(0, 16, 16, 16), 0.1).
		pushFrame(sf::IntRect(16, 16, 16, 16), 0.1).
		pushFrame(sf::IntRect(32, 16, 16, 16), 0.1);
	m_animation.createAnimation("up");
	m_animation.pushFrame(sf::IntRect(0, 48, 16, 16), 0.1).
		pushFrame(sf::IntRect(16, 48, 16, 16), 0.1).
		pushFrame(sf::IntRect(32, 48, 16, 16), 0.1);
	m_animation.createAnimation("down");
	m_animation.pushFrame(sf::IntRect(0, 0, 16, 16), 0.1).
		pushFrame(sf::IntRect(16, 0, 16, 16), 0.1).
		pushFrame(sf::IntRect(32, 0, 16, 16), 0.1);
	m_animation.createAnimation("lookRight");
	m_animation.pushFrame(sf::IntRect(0, 32, 16, 16), 0.1);
	m_animation.createAnimation("lookLeft");
	m_animation.pushFrame(sf::IntRect(0, 16, 16, 16), 0.1);
	m_animation.createAnimation("lookUp");
	m_animation.pushFrame(sf::IntRect(0, 48, 16, 16), 0.1);
	m_animation.createAnimation("lookDown");
	m_animation.pushFrame(sf::IntRect(0, 0, 16, 16), 0.1);

	m_animation.initAnimation(m_sprite, "down");
	m_sprite.setTexture(assets.getAsset<jl::TextureAsset>("res/rpgmaker16.png")->get());
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

		std::vector<AstarNode> path = AstarAlgorithm::findPath(getIndex(), pathGoal, &getTileMap());

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

		if(!isWalking())
			if(m_gameState->getTileMap().getTile(pathIndex).getTileType() == 5 ||
				m_gameState->getTileMap().getTile(pathIndex).getTileType() == 6 ||
				(m_gameState->getTileMap().getTile(pathIndex).isOccupied() && dynamic_cast<Player*>(m_gameState->getTileMap().getTile(pathIndex).getCharacter())))
				m_gameState->getTileMap().getTile(pathIndex).damage(m_damage);

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