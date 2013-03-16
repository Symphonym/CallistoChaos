#ifndef JL_TEXTUREASSET_H
#define JL_TEXTUREASSET_H

#include <SFML/Graphics.hpp>
#include "Asset.h"

namespace jl
{
	class TextureAsset : public Asset
	{
	private:

		sf::Texture m_texture;

	public:

		bool loadAsset(const std::string &filepath);

		// Returns the managed asset
		sf::Texture &get();
	};
};

#endif