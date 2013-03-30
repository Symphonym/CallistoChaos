#ifndef WORKBENCH_H
#define WORKBENCH_H

#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>
#include "Weapon.h"
#include "AssetManager.h"
#include "WorkbenchFunctions.h"


class Player;
class Workbench
{
private:

	jl::AssetManager *m_assets;
	Player *m_player;

	typedef std::function<int (Weapon *weapon, int cost)> WeaponCostCalcPtr;
	typedef std::function<void (std::shared_ptr<Weapon> weapon, Player *player, int cost, bool isFirstTimePurchase, int maxUpgradeLevel)> WeaponActionPtr;

	struct WorkbenchItem
	{
		// Cost calculator
		WeaponCostCalcPtr weaponCostCalc;
		// Action to do when the weapon is purchased/upgraded
		WeaponActionPtr weaponAction;

		// The weapon
		std::shared_ptr<Weapon> weapon;

		std::string name;
		bool isBought;
		sf::IntRect displaySubRect;

		int maxUpgradeLevel, cost;
		sf::Vector2f position;
		sf::Vector2f targetPosition;
	};

	// List of workbench items
	std::vector<WorkbenchItem> m_workbenchItems;
	int m_selectedItem;

	sf::Sprite m_itemBackgroundSprite;
	sf::Sprite m_itemDisplaySprite;
	sf::Sprite m_itemBuySprite;
	sf::Sprite m_itemUpgradeSprite;
	sf::Text m_itemNameText;
	sf::Text m_itemCostText;

	bool m_isVisible;

	int m_itemSpacing;
	sf::IntRect m_boughtSelRect, m_boughtNonRect;
	sf::IntRect m_forsaleSelRect, m_forsaleNonRect;
	sf::IntRect m_buyBoughtRect, m_buyForSaleRect;
	sf::IntRect m_selUpgradeRect, m_nonUpgradeRect;

public:

	explicit Workbench(Player *player, jl::AssetManager &assets);

	void addBuyableWeapon(const sf::IntRect &displaySubRect, int cost, int maxUpgradeLevel, std::shared_ptr<Weapon> weapon, WeaponActionPtr weaponAction = WorkbenchFunctions::defaultPurchase, WeaponCostCalcPtr weaponCostCalc = WorkbenchFunctions::defaultCostCalc);
	void addUpgradeableWeapon(const sf::IntRect &displaySubRect, int cost, int maxUpgradeLevel, std::shared_ptr<Weapon> weapon, WeaponActionPtr weaponAction = WorkbenchFunctions::defaultPurchase, WeaponCostCalcPtr weaponCostCalc = WorkbenchFunctions::defaultCostCalc);

	void events(sf::Event &events);
	void update(double deltaTime);
	void render(sf::RenderTarget &target);

	void setVisible(bool visible);
	bool isVisible() const;

};

#endif