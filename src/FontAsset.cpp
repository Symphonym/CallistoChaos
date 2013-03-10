#include "FontAsset.h"

namespace cc
{
	bool FontAsset::loadAsset(const std::string &filepath)
	{
		return m_font.loadFromFile(filepath);
	}
	
	sf::Font &FontAsset::getFont()
	{
		return m_font;
	}
};