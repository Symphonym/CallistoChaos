#include "FontAsset.h"

namespace jl
{
	bool FontAsset::loadAsset(const std::string &filepath)
	{
		return m_font.loadFromFile(filepath);
	}
	
	sf::Font &FontAsset::get()
	{
		return m_font;
	}
};