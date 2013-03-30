#include "Workbench.h"
#include "Utility.h"

Workbench::Workbench(jl::AssetManager &assets) :
	m_assets(&assets),
	m_selectedItem(0),
	m_isVisible(false),
	m_itemSpacing(0),
	m_selRect(0,0,0,0),
	m_nonRect(0,0,0,0)
{
	m_itemBackgroundSprite.setTexture(assets.getAsset<jl::TextureAsset>("res/gui.png")->get());
	m_itemBackgroundSprite.setScale(3,3);sf::Text text;

	m_itemNameText.setFont(assets.getAsset<jl::FontAsset>("res/Minecraftia.ttf")->get());
	m_itemNameText.setCharacterSize(12);
	m_itemNameText.setString("Pulse Rifle");

	m_selRect = sf::IntRect(0, 12, 66, 19);
	m_nonRect = sf::IntRect(0, 31, 66, 19);

	m_itemSpacing = 50;

	for(int i = 0; i < 10; i++)
	{
		WorkbenchItem item;
		m_workbenchItems.push_back(item);
	}
}

void Workbench::events(sf::Event &events)
{
	if(m_isVisible)
	{
		// B button to leave menu
		if(jl::Input::isButtonDown(events, 1))
		{
			m_isVisible = false;
			return;
		}

		int yModifier = 0;
		if(jl::Input::isAxisDown(events, sf::Joystick::Axis::PovY, -100))
		{
			yModifier = (m_itemBackgroundSprite.getGlobalBounds().height + m_itemSpacing);
			--m_selectedItem;
		}
		else if(jl::Input::isAxisDown(events, sf::Joystick::Axis::PovY, 100))
		{
			yModifier = -(m_itemBackgroundSprite.getGlobalBounds().height + m_itemSpacing);
			++m_selectedItem;
		}

		if(m_selectedItem < 0)
		{
			m_selectedItem = 0;
			yModifier = 0;
		}
		else if(m_selectedItem >= m_workbenchItems.size())
		{
			m_selectedItem = m_workbenchItems.size() - 1;
			yModifier = 0;
		}

		for(std::size_t i = 0; i < m_workbenchItems.size(); i++)
			m_workbenchItems[i].targetPosition.y += yModifier;
	}
}
void Workbench::update(double deltaTime)
{
	if(m_isVisible)
	{
		for(std::size_t i = 0; i < m_workbenchItems.size(); i++)
		{
			m_workbenchItems[i].position = jl::Vec::lerp(
				m_workbenchItems[i].position, m_workbenchItems[i].targetPosition,
				 10*deltaTime);
		}
	}
}

void Workbench::render(sf::RenderTarget &target)
{
	if(m_isVisible)
	{
		for(std::size_t i = 0; i < m_workbenchItems.size(); i++)
		{

			m_itemBackgroundSprite.setTextureRect(i == m_selectedItem ? m_selRect : m_nonRect);
			m_itemBackgroundSprite.setPosition(
				target.getView().getSize().x/2 - m_itemBackgroundSprite.getGlobalBounds().width/2,
				100 + m_workbenchItems[i].position.y + (i * (m_itemBackgroundSprite.getGlobalBounds().height + m_itemSpacing)));
			target.draw(m_itemBackgroundSprite);

			sf::View tempView = target.getView();
			target.setView(target.getDefaultView());

			m_itemNameText.setPosition(
				(m_itemBackgroundSprite.getPosition().x + m_itemBackgroundSprite.getGlobalBounds().width) - 50,
				m_itemBackgroundSprite.getPosition().y + 20);
			m_itemNameText.setPosition((sf::Vector2f)target.mapCoordsToPixel(m_itemNameText.getPosition(), tempView));
			target.draw(m_itemNameText);

			target.setView(tempView);
		}
	}
}

void Workbench::setVisible(bool visible)
{
	m_isVisible = visible;
}
bool Workbench::isVisible() const
{
	return m_isVisible;
}