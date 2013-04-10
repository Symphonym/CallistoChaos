#include "Workbench.h"
#include "Utility.h"
#include "Weapon.h"
#include "GunWeapon.h"
#include "RifleWeapon.h"
#include "Player.h"
#include "Settings.h"
#include "MessageLog.h"

Workbench::Workbench() :
	m_assets(nullptr),
	m_player(nullptr),
	m_selectedItem(0),
	m_isVisible(false),
	m_itemSpacing(0),
	m_boughtSelRect(0,0,0,0),
	m_boughtNonRect(0,0,0,0),
	m_forsaleSelRect(0,0,0,0),
	m_forsaleNonRect(0,0,0,0),
	m_buyBoughtRect(0,0,0,0),
	m_buyForSaleRect(0,0,0,0)
{

}

Workbench::Workbench(Player *player, jl::AssetManager &assets) :
	m_assets(&assets),
	m_player(player),
	m_selectedItem(0),
	m_isVisible(false),
	m_itemSpacing(0),
	m_boughtSelRect(0,0,0,0),
	m_boughtNonRect(0,0,0,0),
	m_forsaleSelRect(0,0,0,0),
	m_forsaleNonRect(0,0,0,0),
	m_buyBoughtRect(0,0,0,0),
	m_buyForSaleRect(0,0,0,0)
{
	m_itemDisplaySprite.setTexture(assets.getAsset<jl::TextureAsset>("res/weapons.png")->get());
	m_itemDisplaySprite.setScale(2, 2);

	m_itemBackgroundSprite.setTexture(assets.getAsset<jl::TextureAsset>("res/gui.png")->get());
	m_itemBackgroundSprite.setScale(3,3);sf::Text text;

	m_itemBuySprite.setTexture(assets.getAsset<jl::TextureAsset>("res/gui.png")->get());
	m_itemBuySprite.setScale(2,2);

	m_itemUpgradeSprite.setTexture(assets.getAsset<jl::TextureAsset>("res/gui.png")->get());
	m_itemUpgradeSprite.setScale(3,3);

	m_itemNameText.setFont(assets.getAsset<jl::FontAsset>("res/Minecraftia.ttf")->get());
	m_itemNameText.setCharacterSize(std::floor(20*jl::Settings::getDouble("gameRatio")));

	m_itemCostText.setFont(assets.getAsset<jl::FontAsset>("res/Minecraftia.ttf")->get());
	m_itemCostText.setCharacterSize(std::floor(15*jl::Settings::getDouble("gameRatio")));

	m_boughtSelRect = sf::IntRect(0, 12, 66, 19);
	m_boughtNonRect = sf::IntRect(0, 31, 66, 19);
	m_forsaleSelRect = sf::IntRect(0, 50, 66, 19);
	m_forsaleNonRect = sf::IntRect(0, 69, 66, 19);

	m_buyBoughtRect = sf::IntRect(70, 11, 25, 18);
	m_buyForSaleRect = sf::IntRect(70, 29, 25, 18);

	m_nonUpgradeRect = sf::IntRect(67,48,30,5);
	m_selUpgradeRect = sf::IntRect(67,54,30,5);


	m_itemSpacing = 50;

	// Ammo purchase
	WorkbenchItem ammoItem;
	ammoItem.weaponCostCalc = WorkbenchFunctions::ammoCostCalc;
	ammoItem.weaponAction = WorkbenchFunctions::ammoPurchase;
	ammoItem.weapon = std::shared_ptr<Weapon>(nullptr);
	ammoItem.name = "Batteries x5";
	ammoItem.isBought = true;
	ammoItem.displaySubRect = sf::IntRect(23, 49, 6, 8);
	ammoItem.maxUpgradeLevel = -1;
	ammoItem.upgradeLevel = -1;
	ammoItem.cost = 1;
	m_workbenchItems.push_back(ammoItem);

	// Health purchase
	WorkbenchItem healthItem;
	healthItem.weaponCostCalc = WorkbenchFunctions::defaultCostCalc;
	healthItem.weaponAction = WorkbenchFunctions::healthPurchase;
	healthItem.weapon = std::shared_ptr<Weapon>(nullptr);
	healthItem.name = "Health pack";
	healthItem.isBought = true;
	healthItem.displaySubRect = sf::IntRect(30, 49, 7, 7);
	healthItem.maxUpgradeLevel = 4;
	healthItem.upgradeLevel = 1;
	healthItem.cost = 50;
	m_workbenchItems.push_back(healthItem);

	// Speed purchase
	WorkbenchItem speedItem;
	speedItem.weaponCostCalc = WorkbenchFunctions::defaultCostCalc;
	speedItem.weaponAction = WorkbenchFunctions::speedPurchase;
	speedItem.weapon = std::shared_ptr<Weapon>(nullptr);
	speedItem.name = "Speed boost";
	speedItem.isBought = true;
	speedItem.displaySubRect = sf::IntRect(30, 49, 7, 7);
	speedItem.maxUpgradeLevel = 4;
	speedItem.upgradeLevel = 1;
	speedItem.cost = 150;
	m_workbenchItems.push_back(speedItem);

	std::shared_ptr<Weapon> defaultWeapon(new GunWeapon("Pulse Pistol", m_player, *m_assets));
	addUpgradeableWeapon(sf::IntRect(0,0,16,16),50, 5, defaultWeapon);
	m_player->addWeapon(defaultWeapon);
	addBuyableWeapon(sf::IntRect(0,48,22,9),150, 5, std::shared_ptr<Weapon>(new RifleWeapon("Pulse Rifle", m_player, *m_assets)));

}


void Workbench::addBuyableWeapon(const sf::IntRect &displaySubRect, int cost, int maxUpgradeLevel, std::shared_ptr<Weapon> weapon, WeaponActionPtr weaponAction, WeaponCostCalcPtr weaponCostCalc)
{
	WorkbenchItem item;
	item.weaponCostCalc = weaponCostCalc;
	item.weaponAction = weaponAction;
	item.weapon = weapon;
	item.name = weapon->getName();
	item.isBought = false;
	item.displaySubRect = displaySubRect;
	item.maxUpgradeLevel = maxUpgradeLevel;
	item.upgradeLevel = -1;
	item.cost = cost;
	item.position = sf::Vector2f(0,0);
	item.targetPosition = sf::Vector2f(0,0);

	m_workbenchItems.push_back(item);
}
void Workbench::addUpgradeableWeapon(const sf::IntRect &displaySubRect, int cost, int maxUpgradeLevel, std::shared_ptr<Weapon> weapon, WeaponActionPtr weaponAction, WeaponCostCalcPtr weaponCostCalc)
{
	WorkbenchItem item;
	item.weaponCostCalc = weaponCostCalc;
	item.weaponAction = weaponAction;
	item.weapon = weapon;
	item.name = weapon->getName();
	item.isBought = true;
	item.displaySubRect = displaySubRect;
	item.maxUpgradeLevel = maxUpgradeLevel;
	item.upgradeLevel = -1;
	item.cost = cost;
	item.position = sf::Vector2f(0,0);
	item.targetPosition = sf::Vector2f(0,0);

	m_workbenchItems.push_back(item);
}




void Workbench::events(sf::Event &events)
{
	if(m_isVisible)
	{
		// B button to leave menu
		if(jl::Input::isButtonDown(events, 1))
		{
			m_player->setBusy(false);
			m_isVisible = false;
			return;
		}

		// A button to buy
		else if(jl::Input::isButtonDown(events, 0))
		{
			int cost = getSelectedItem()->weaponCostCalc(
				getSelectedItem()->upgradeLevel,
				getSelectedItem()->cost);

			if(getSelectedItem()->weapon.get() != nullptr)
				cost = getSelectedItem()->weaponCostCalc(getSelectedItem()->weapon->getLevel(), getSelectedItem()->cost);
			
			bool canBuy = true;

			// Check if is max upgraded
			if((getSelectedItem()->weapon.get() != nullptr && getSelectedItem()->isBought && getSelectedItem()->weapon->getLevel() >= getSelectedItem()->maxUpgradeLevel && getSelectedItem()->maxUpgradeLevel > 0) ||
				(getSelectedItem()->weapon.get() == nullptr && getSelectedItem()->isBought && getSelectedItem()->upgradeLevel >= getSelectedItem()->maxUpgradeLevel && getSelectedItem()->maxUpgradeLevel > 0 && getSelectedItem()->upgradeLevel > 0))
			{
					canBuy = false;

					if(getSelectedItem()->weapon.get() != nullptr)
						MessageLog::addSingleMessage("The " + getSelectedItem()->weapon->getName() + " is already fully upgraded");
					else
						MessageLog::addSingleMessage("This upgrade is already fully upgraded");
			}


			// Check if player can afford
			if(m_player->affordCurrency(cost) && canBuy)
			{
				// Check if it's a non upgradeable, if not, check if it's upgradeable any more otherwise just buy it
				if(canBuy)
				{
					getSelectedItem()->weaponAction(*getSelectedItem(), m_player);

					getSelectedItem()->isBought = true;
				}
			}
			else if(canBuy)
				MessageLog::addSingleMessage("I don't have enough material");
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

			// Draw background of item
			if(m_workbenchItems[i].isBought)
				m_itemBackgroundSprite.setTextureRect(i == m_selectedItem ? m_boughtSelRect : m_boughtNonRect);
			else
				m_itemBackgroundSprite.setTextureRect(i == m_selectedItem ? m_forsaleSelRect : m_forsaleNonRect);

			m_itemBackgroundSprite.setPosition(
				target.getView().getSize().x/2 - m_itemBackgroundSprite.getGlobalBounds().width/2,
				100 + m_workbenchItems[i].position.y + (i * (m_itemBackgroundSprite.getGlobalBounds().height + m_itemSpacing)));
			target.draw(m_itemBackgroundSprite);

			// Set hardcoded bounds of the upgrade box
			sf::Vector2f upgradeBoxSize(
				30.0*m_itemBackgroundSprite.getScale().x,
				5.0*m_itemBackgroundSprite.getScale().y);
			sf::Vector2f upgradeBoxStart(
				4.0*m_itemBackgroundSprite.getScale().x,
				11.0*m_itemBackgroundSprite.getScale().y);

			// Display upgrade progress
			if(m_workbenchItems[i].maxUpgradeLevel > 0)
			{
				m_itemUpgradeSprite.setTextureRect(m_selUpgradeRect);

				// Disregard first upgrade level as it doesn't really do anything
				int level = m_workbenchItems[i].upgradeLevel;

				if(m_workbenchItems[i].weapon.get() != nullptr)
					level = m_workbenchItems[i].weapon->getLevel();

				m_itemUpgradeSprite.setScale(
					((((double)level-1.0)/((double)m_workbenchItems[i].maxUpgradeLevel-1.0))*upgradeBoxSize.x),
					3.0);

				m_itemUpgradeSprite.setPosition(
				m_itemBackgroundSprite.getPosition().x + upgradeBoxStart.x,
				m_itemBackgroundSprite.getPosition().y + upgradeBoxStart.y);
					target.draw(m_itemUpgradeSprite);
			}

			sf::View tempView = target.getView();
			target.setView(target.getDefaultView());

			// Draw weapon name and upgrade level
			sf::Vector2i convertedTextPos(target.mapCoordsToPixel(
				sf::Vector2f(
					m_itemBackgroundSprite.getPosition().x+10.5,
					m_itemBackgroundSprite.getPosition().y+5),
				tempView));
			m_itemNameText.setPosition(convertedTextPos.x, convertedTextPos.y);
			m_itemNameText.setString(m_workbenchItems[i].name);
			target.draw(m_itemNameText);

			if((m_workbenchItems[i].weapon.get() == nullptr && m_workbenchItems[i].upgradeLevel < 0) || 
				(m_workbenchItems[i].weapon.get() == nullptr && m_workbenchItems[i].maxUpgradeLevel < 0))
				m_itemNameText.setString("N/A");
			else
			{
				int level = m_workbenchItems[i].upgradeLevel;
				if(m_workbenchItems[i].weapon.get() != nullptr)
					level = m_workbenchItems[i].weapon->getLevel();

				m_itemNameText.setString(
					jl::Util::toString(level-1)+"/"
					+jl::Util::toString(m_workbenchItems[i].maxUpgradeLevel-1));
			}

			sf::Vector2i convertedLevelPos(target.mapCoordsToPixel(
				sf::Vector2f(
					m_itemBackgroundSprite.getPosition().x + upgradeBoxStart.x + upgradeBoxSize.x/2,
					m_itemBackgroundSprite.getPosition().y+ upgradeBoxStart.y + upgradeBoxSize.y/2),
				tempView));
			m_itemNameText.setPosition(
				convertedLevelPos.x - m_itemNameText.getGlobalBounds().width/2, 
				convertedLevelPos.y - m_itemNameText.getGlobalBounds().height/2);

			target.draw(m_itemNameText);

			target.setView(tempView);

			// Set hardcoded bounds of display box
			sf::Vector2f displayBoxSize(
				26.0*m_itemBackgroundSprite.getScale().x,
				13.0*m_itemBackgroundSprite.getScale().y);
			sf::Vector2f displayBoxStart(
				37.0*m_itemBackgroundSprite.getScale().x,
				3.0*m_itemBackgroundSprite.getScale().y);

			// Display weapon
			m_itemDisplaySprite.setTextureRect(m_workbenchItems[i].displaySubRect);
			m_itemDisplaySprite.setPosition(
			 (m_itemBackgroundSprite.getPosition().x + displayBoxStart.x + displayBoxSize.x/2) - (m_itemDisplaySprite.getGlobalBounds().width/2),
			 (m_itemBackgroundSprite.getPosition().y + displayBoxStart.y + displayBoxSize.y/2) - (m_itemDisplaySprite.getGlobalBounds().height/2));
			target.draw(m_itemDisplaySprite);

			// Display buy button
			if(i == m_selectedItem)
			{
				m_itemBuySprite.setTextureRect(m_workbenchItems[i].isBought ? m_buyBoughtRect : m_buyForSaleRect);

				m_itemBuySprite.setPosition(
					m_itemBackgroundSprite.getPosition().x + m_itemBackgroundSprite.getGlobalBounds().width + 3,
					(m_itemBackgroundSprite.getPosition().y + m_itemBackgroundSprite.getGlobalBounds().height/2) - m_itemBuySprite.getGlobalBounds().height/2);
				target.draw(m_itemBuySprite);

				// Display cost
				sf::View tempView = target.getView();
				target.setView(target.getDefaultView());

				sf::Vector2f costTextPos(
					m_itemBuySprite.getPosition().x + 24, m_itemBuySprite.getPosition().y + 21);

				sf::Vector2i convertedCostTextPos(target.mapCoordsToPixel(costTextPos, tempView));
				m_itemCostText.setPosition(convertedCostTextPos.x, convertedCostTextPos.y);

				if((getSelectedItem()->weapon.get() != nullptr && getSelectedItem()->weapon->getLevel() >= getSelectedItem()->maxUpgradeLevel && getSelectedItem()->maxUpgradeLevel > 0) ||
					(getSelectedItem()->weapon.get() == nullptr && getSelectedItem()->upgradeLevel >= getSelectedItem()->maxUpgradeLevel && getSelectedItem()->upgradeLevel > 0 && getSelectedItem()->maxUpgradeLevel > 0))
					m_itemCostText.setString("N/A");
				else
				{
					int cost = getSelectedItem()->weaponCostCalc(getSelectedItem()->upgradeLevel, getSelectedItem()->cost);

					if(getSelectedItem()->weapon.get() != nullptr)
						cost = getSelectedItem()->weaponCostCalc(getSelectedItem()->weapon->getLevel(), getSelectedItem()->cost);
					
					m_itemCostText.setString(jl::Util::toString(cost));
				}
				target.draw(m_itemCostText);
				target.setView(tempView);
			}
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
WorkbenchItem *Workbench::getSelectedItem()
{
	return &m_workbenchItems[m_selectedItem];
}