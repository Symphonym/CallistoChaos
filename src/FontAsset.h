#ifndef CC_FONTASSET_H
#define CC_FONTASSET_H

#include <SFML/Graphics.hpp>
#include "Asset.h"

namespace cc
{
	class FontAsset : public Asset
	{
	private:

		sf::Font m_font;

	public:

		bool loadAsset(const std::string &filepath);
		sf::Font &getFont();
	};
};

#endif