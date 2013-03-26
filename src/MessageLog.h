#ifndef MESSAGELOG_H
#define MESSAGELOG_H

#include <SFML/Graphics.hpp>
#include "AssetManager.h"

namespace MessageLog
{
	void addMessage(const std::string &message);
	void addSingleMessage(const std::string &message);
	void loadAssets(jl::AssetManager &assets);

	void update(double deltaTime);
	void render(sf::RenderTarget &target);
};

#endif