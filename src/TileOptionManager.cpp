#include "TileOptionManager.h"
#include "TileMap.h"
#include "Utility.h"
#include "Settings.h"
#include "SoundManager.h"
#include "XboxInput.h"

TileOptionManager::TileOptionManager() :
	m_tileOptions(),
	m_player(nullptr),
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

	m_sprite.setTexture(assets.getTexture("res/gui.png"));
	m_sprite.setScale(3, 2);
	m_text.setFont(assets.getFont("res/Minecraftia.ttf"));
	m_text.setCharacterSize(8);
	jl::SoundManager::addSound("res/tileoptionselect.wav");
	jl::SoundManager::addSound("res/tileoptionconfirm.wav");
	jl::SoundManager::getSound("res/tileoptionconfirm.wav").setVolume(50);
}

void TileOptionManager::addOption(int tileTypeIndex, const std::string &title, ActionPtr action)
{
	m_tileOptions[tileTypeIndex].push_back(std::make_pair(title, action));
}
void TileOptionManager::insertOption(int tileTypeIndex, const std::string &title, ActionPtr action, int optionIndex)
{
	m_tileOptions[tileTypeIndex].insert(m_tileOptions[tileTypeIndex].begin() + optionIndex, std::make_pair(title, action));
}
void TileOptionManager::removeOption(int tileTypeIndex, int optionIndex)
{
	m_tileOptions[tileTypeIndex].erase(m_tileOptions[tileTypeIndex].begin() + optionIndex);
}

void TileOptionManager::provideCharacter(Player *player)
{
	m_player = player;
}
void TileOptionManager::events(sf::Event &events)
{
	if(m_player->isBusy())
		m_displayOptions = false;
	else
	{
		// Interact with tile
		if(events.type == sf::Event::JoystickButtonPressed)
		{
			int tileType = getTileType();


			// Display list
			//if(jl::Input::isButtonDown(events, 0) && !m_tileOptions[tileType].empty())
			if(jl::XboxInput::isButtonDown(0, jl::XboxInput::Buttons::A))
			{
				jl::SoundManager::playSound("res/tileoptionconfirm.wav");

				if(!m_displayOptions)
					displayList();
				// Run option command
				else if(m_displayOptions)
				{
					m_tileOptions[m_tileType][m_optionIndex].second(
						&TileOptionManager::m_player->getTileMap(),
						m_tileIndex,
						this);

					// Check if the aciton changed type of the Tile
					if(tileType != getTileType())
						displayList();

				}

			}
			else if(m_displayOptions)
			{
				// Other action not accounted for, simply close the list
				m_displayOptions = false;

				//m_optionIndex = jl::Math::clamp<int,int,int>(m_optionIndex,0, m_tileOptions[m_tileType].size() - 1);
			}
		}

		else if(events.type == sf::Event::JoystickMoved && m_displayOptions)
		{
			
			// Scroll up
			//if(sf::Joystick::getAxisPosition(jl::XboxInput::translateAxis(jl::XboxInput::Axis::DPadY)) == -100)
			if(((jl::XboxInput::isAxisDown(0, jl::XboxInput::Axis::DPadY, -100, 25) && jl::XboxInput::usingUnix()) ||
			(jl::XboxInput::isAxisDown(0, jl::XboxInput::Axis::DPadY, 100, 25) && jl::XboxInput::usingWindows())) && 
			jl::Input::axisMoved(getEngine()->getEvent(), jl::XboxInput::translateAxis(jl::XboxInput::Axis::DPadY)))
			{
				--m_optionIndex;
				jl::SoundManager::playSound("res/tileoptionselect.wav");
			}
			// Scroll down
			//else if (jl::Input::isAxisDown(events, sf::Joystick::Axis::PovY, 100))
			else if(((jl::XboxInput::isAxisDown(0, jl::XboxInput::Axis::DPadY, 100, 25) && jl::XboxInput::usingUnix()) ||
			(jl::XboxInput::isAxisDown(0, jl::XboxInput::Axis::DPadY, -100, 25) && jl::XboxInput::usingWindows())) && 
			jl::Input::axisMoved(getEngine()->getEvent(), jl::XboxInput::translateAxis(jl::XboxInput::Axis::DPadY)))
			{
				++m_optionIndex;
				jl::SoundManager::playSound("res/tileoptionselect.wav");
			}

			
			m_optionIndex = jl::Math::clamp<int,int,int>(m_optionIndex,0, m_tileOptions[m_tileType].size() - 1);
		}
	}
}
void TileOptionManager::render(sf::RenderTarget &target)
{
	if(m_player->isBusy())
		m_displayOptions = false;
	else
	{
		if(m_displayOptions)
		{
			for(int i = 0; i < (int)m_tileOptions[m_tileType].size(); i++)
			{
				std::size_t tileSize(getPlayer()->getTileMap().getTileSize());
				sf::Vector2i mapSize(getPlayer()->getTileMap().getMapSize());

				sf::Vector2f tilePos(getPlayer()->getTileMap().getTilePosition(m_tileIndex.x, m_tileIndex.y));
				sf::Vector2f listPos(tilePos.x + tileSize, tilePos.y);

				// Place list to the left of tile if there's no right side
				if(m_tileIndex.x == mapSize.x - 1)
					listPos.x -= tileSize * 2;

				m_sprite.setTextureRect(i == m_optionIndex ? m_selRect : m_nonRect);

				// Convert position to pixel
				sf::Vector2i convertedPos(target.mapCoordsToPixel(listPos, target.getView()));
				convertedPos.y += (i * m_sprite.getGlobalBounds().height);
				m_sprite.setPosition(convertedPos.x, convertedPos.y);

				m_text.setPosition(convertedPos.x + 3, convertedPos.y + 1);
				m_text.setString(m_tileOptions[m_tileType][i].first);

				sf::View tempView = target.getView();
				target.setView(target.getDefaultView());

				target.draw(m_sprite);

				target.draw(m_text);
				target.setView(tempView);
			}
		}
	}
}

void TileOptionManager::setVisible(bool visible)
{
	if(visible)
		displayList();
	else
		m_displayOptions = visible;
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
