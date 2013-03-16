#ifndef JL_FONTASSET_H
#define JL_FONTASSET_H

#include <SFML/Graphics.hpp>
#include "Asset.h"

namespace jl
{
	class FontAsset : public Asset
	{
	private:

		sf::Font m_font;

	public:

		bool loadAsset(const std::string &filepath);

		// Returns the managed asset
		sf::Font &get();
	};
};

#endif