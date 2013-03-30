#ifndef WORKBENCH_H
#define WORKBENCH_H

#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>
#include "Weapon.h"
#include "AssetManager.h"

class Workbench
{
private:

	typedef std::function<std::unique_ptr<Weapon>(jl::AssetManager &assets)> WeaponFactoryPtr;
	jl::AssetManager *m_assets;

	struct WorkbenchItem
	{
		// Cost and factory function for weapon
		std::pair<int, WeaponFactoryPtr> buyData;

		// Pointer to the weapon after it's bought
		// nullptr otherwise
		Weapon *weapon;

		int maxUpgradeLevel;
		sf::Vector2f position;
		sf::Vector2f targetPosition;
	};

	// List of workbench items
	std::vector<WorkbenchItem> m_workbenchItems;
	int m_selectedItem;

	sf::Sprite m_itemBackgroundSprite;
	sf::Text m_itemNameText;

	bool m_isVisible;

	int m_itemSpacing;
	sf::IntRect m_selRect, m_nonRect;

public:

	explicit Workbench(jl::AssetManager &assets);

	void events(sf::Event &events);
	void update(double deltaTime);
	void render(sf::RenderTarget &target);

	void setVisible(bool visible);
	bool isVisible() const;

};

#endif