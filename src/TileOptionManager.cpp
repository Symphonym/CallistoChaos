#include "TileOptionManager.h"
#include "TileMap.h"
#include "Utility.h"

TileOptionManager::TileOptionManager() :
	m_tileOptions(),
	m_tileCharacter(nullptr),
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
	m_selRect = sf::IntRect(0,0,25,6);
	m_nonRect = sf::IntRect(0,6,25,6);

	m_sprite.setTexture(assets.getAsset<jl::TextureAsset>("res/gui.png")->get());
	m_text.setFont(assets.getAsset<jl::FontAsset>("res/Minecraftia.ttf")->get());
	m_text.setCharacterSize(8);
}

void TileOptionManager::addOption(int tileTypeIndex, const std::string &title, std::function<void()> action)
{
	m_tileOptions[tileTypeIndex].push_back(std::make_pair(title, action));
}

void TileOptionManager::provideCharacter(TileCharacter *tilecharacter)
{
	m_tileCharacter = tilecharacter;
}

void TileOptionManager::events(sf::Event &events)
{
	// Interact with tile
	if(events.type == sf::Event::KeyPressed)
	{
		// Display list
		if(events.key.code == sf::Keyboard::U)
		{
			if(!m_displayOptions)
			{
				sf::Vector2i tileIndex(getFacingTile());
				tileIndex.x = jl::Math::clamp(tileIndex.x, 0, m_tileCharacter->getTileMap().getMapSize().x - 1);
				tileIndex.y = jl::Math::clamp(tileIndex.y, 0, m_tileCharacter->getTileMap().getMapSize().y - 1);
				int tileType = m_tileCharacter->getTileMap().getTile(tileIndex.x, tileIndex.y).getTileType();

				m_displayOptions = true;
				m_tileIndex = getFacingTile();
				m_tileType = tileType;
				m_optionIndex = m_tileOptions[m_tileType].size() - 1;
			}

			// Run option command
			else
				m_tileOptions[m_tileType][m_optionIndex].second();

		}
		else if(m_displayOptions)
		{
			// Scroll up
			if(events.key.code == sf::Keyboard::O)
				--m_optionIndex;
			// Scroll down
			else if(events.key.code == sf::Keyboard::P)
				++m_optionIndex;
			// Other action not accounted for, simply close the list
			else
				m_displayOptions = false;

			m_optionIndex = jl::Math::clamp<int,int,int>(m_optionIndex,0, m_tileOptions[m_tileType].size() - 1);
		}
	}
}
void TileOptionManager::render(sf::RenderTarget &target)
{
	if(m_displayOptions)
	{
		for(int i = 0; i < (int)m_tileOptions[m_tileType].size(); i++)
		{
			std::size_t tileSize(m_tileCharacter->getTileMap().getTileSize());
			sf::Vector2i mapSize(m_tileCharacter->getTileMap().getMapSize());

			sf::Vector2f tilePos(m_tileCharacter->getTileMap().getTilePosition(m_tileIndex.x, m_tileIndex.y));
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
	sf::Vector2i charIndex(m_tileCharacter->getIndex());
	TileCharacter::Directions direction = m_tileCharacter->getDirection();

	if(direction == TileCharacter::IdleRight)
		return sf::Vector2i(charIndex.x + 1, charIndex.y);
	else if(direction == TileCharacter::IdleLeft)
		return sf::Vector2i(charIndex.x - 1, charIndex.y);
	else if(direction == TileCharacter::IdleUp)
		return sf::Vector2i(charIndex.x, charIndex.y - 1);
	else if(direction == TileCharacter::IdleDown)
		return sf::Vector2i(charIndex.x, charIndex.y + 1);
	else
		return sf::Vector2i(-1, -1);
}