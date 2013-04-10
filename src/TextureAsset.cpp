#include "TextureAsset.h"

namespace jl
{

	bool TextureAsset::loadAsset(const std::string &filepath)
	{
		return m_texture.loadFromFile(filepath);
	}

	sf::Texture &TextureAsset::get()
	{
		return m_texture;
	}
};