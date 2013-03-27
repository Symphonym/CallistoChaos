#include "TileOptionManager.h"
#include "TileMap.h"
#include "Utility.h"

Player *TileOptionManager::m_player = nullptr;

TileOptionManager::TileOptionManager() :
	m_tileOptions(),
	m_displayOptions(false),
	m_optionIndex(0),
	m_tileIndex(0,0),
	m_tileType(0),
	m_sprite(),
	m_selRect(0,0,0,0),
	m_nonRect(0,0,0,0),
	m_text()
{

}

void TileOptionManager::loadAssets(jl::AssetManager &assets)
{
	m_nonRect = sf::IntRect(0,0,25,6);
	m_selRect = sf::IntRect(0,6,25,6);

	m_sprite.setTexture(assets.getAsset<jl::TextureAsset>("res/gui.png")->get());
	m_text.setFont(assets.getAsset<jl::FontAsset>("res/Minecraftia.ttf")->get());
	m_text.setCharacterSize(8);
}

void TileOptionManager::addOption(int tileTypeIndex, const std::string &title, ActionPtr action)
{
	m_tileOptions[tileTypeIndex].push_back(std::make_pair(title, action));
}

void TileOptionManager::provideCharacter(Player *player)
{
	m_player = player;
}

void TileOptionManager::events(sf::Event &events)
{
	if(m_player->isWalking())
		m_displayOptions = false;

	// Interact with tile
	if(events.type == sf::Event::KeyPressed || 
		events.type == sf::Event::JoystickButtonPressed)
	{
		int tileType = getTileType();


		// Display list
		if((jl::Input::isKeyDown(events, sf::Keyboard::G) && !m_tileOptions[tileType].empty()) ||
			(jl::Input::isButtonDown(events, 0) && !m_tileOptions[tileType].empty()))
		{
			if(!m_displayOptions)
				displayList();

			// Run option command
			else if(m_displayOptions)
			{
				m_tileOptions[m_tileType][m_optionIndex].second(
					&TileOptionManager::m_player->getTileMap(),
					m_tileIndex);

				// Check if the aciton changed type of the Tile
				if(tileType != getTileType())
					displayList();

			}

		}
		else if(m_displayOptions)
		{
			// Scroll up
			if(events.key.code == sf::Keyboard::F)
				--m_optionIndex;
			// Scroll down
			else if(events.key.code == sf::Keyboard::V)
				++m_optionIndex;
			// Other action not accounted for, simply close the list
			else
				m_displayOptions = false;

			m_optionIndex = jl::Math::clamp<int,int,int>(m_optionIndex,0, m_tileOptions[m_tileType].size() - 1);
		}
	}

	if(events.type == sf::Event::JoystickMoved && m_displayOptions)
	{
		// Scroll up
		if(jl::Input::isAxisDown(events, sf::Joystick::Axis::PovY, -100))
			--m_optionIndex;
		// Scroll down
		else if (jl::Input::isAxisDown(events, sf::Joystick::Axis::PovY, 100))
			++m_optionIndex;

		m_optionIndex = jl::Math::clamp<int,int,int>(m_optionIndex,0, m_tileOptions[m_tileType].size() - 1);
	}
}
void TileOptionManager::render(sf::RenderTarget &target)
{
	if(m_player->isWalking())
		m_displayOptions = false;

	if(m_displayOptions)
	{
		for(int i = 0; i < (int)m_tileOptions[m_tileType].size(); i++)
		{
			std::size_t tileSize(TileOptionManager::getPlayer()->getTileMap().getTileSize());
			sf::Vector2i mapSize(TileOptionManager::getPlayer()->getTileMap().getMapSize());

			sf::Vector2f tilePos(TileOptionManager::getPlayer()->getTileMap().getTilePosition(m_tileIndex.x, m_tileIndex.y));
			sf::Vector2f listPos(tilePos.x + tileSize, tilePos.y);

			// Place list to the left of tile if there's no right side
			if(m_tileIndex.x == mapSize.x - 1)
				listPos.x -= tileSize * 2;

			m_sprite.setTextureRect(i == m_optionIndex ? m_selRect : m_nonRect);
			m_sprite.setPosition(listPos.x,listPos.y + (i * m_sprite.getTextureRect().height));

			// Converted text position
			sf::Vector2i textPos(target.mapCoordsToPixel(m_sprite.getPosition(), target.getView()));
			m_text.setPosition(textPos.x + 3, textPos.y + 1);
			m_text.setString(m_tileOptions[m_tileType][i].first);

			target.draw(m_sprite);

			sf::View tempView = target.getView();
			target.setView(target.getDefaultView());

			target.draw(m_text);
			target.setView(tempView);
		}
	}
}

bool TileOptionManager::isVisible()
{
	return m_displayOptions;
}

sf::Vector2i TileOptionManager::getFacingTile()
{
	sf::Vector2i charIndex(TileOptionManager::getPlayer()->getIndex());
	TileCharacter::Event direction = TileOptionManager::getPlayer()->getDirection();

	if(direction == TileCharacter::LookingRight)
		return sf::Vector2i(charIndex.x + 1, charIndex.y);
	else if(direction == TileCharacter::LookingLeft)
		return sf::Vector2i(charIndex.x - 1, charIndex.y);
	else if(direction == TileCharacter::LookingUp)
		return sf::Vector2i(charIndex.x, charIndex.y - 1);
	else if(direction == TileCharacter::LookingDown)
		return sf::Vector2i(charIndex.x, charIndex.y + 1);
	else
		return sf::Vector2i(-1, -1);
}
int TileOptionManager::getTileType()
{	
	sf::Vector2i tileIndex(getFacingTile());
	tileIndex.x = jl::Math::clamp(tileIndex.x, 0, TileOptionManager::getPlayer()->getTileMap().getMapSize().x - 1);
	tileIndex.y = jl::Math::clamp(tileIndex.y, 0, TileOptionManager::getPlayer()->getTileMap().getMapSize().y - 1);
	return TileOptionManager::getPlayer()->getTileMap().getTile(tileIndex.x, tileIndex.y).getTileType();
}
void TileOptionManager::displayList()
{
	m_displayOptions = true;
	m_tileIndex = getFacingTile();
	m_tileType = getTileType();
	m_optionIndex = 0;
}


Player *TileOptionManager::getPlayer()
{
	return TileOptionManager::m_player;
}