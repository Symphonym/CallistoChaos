#include "TextureAsset.h"
#include <iostream>
namespace cc
{

	bool TextureAsset::loadAsset(const std::string &filepath)
	{
		return m_texture.loadFromFile(filepath);
	}

	sf::Texture &TextureAsset::getTexture()
	{
		return m_texture;
	}
};