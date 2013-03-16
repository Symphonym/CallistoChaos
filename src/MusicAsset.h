#ifndef JL_MUSICASSET_H
#define JL_MUSICASSET_H

#include <SFML/Audio.hpp>
#include "Asset.h"

namespace jl
{
	class MusicAsset : public Asset
	{
	private:

		sf::Music m_music;

	public:

		bool loadAsset(const std::string &filepath);

		// Returns the managed asset
		sf::Music &get();
	};
};

#endif