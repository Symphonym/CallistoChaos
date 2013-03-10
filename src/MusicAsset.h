#ifndef CC_MUSICASSET_H
#define CC_MUSICASSET_H

#include <SFML/Audio.hpp>
#include "Asset.h"

namespace cc
{
	class MusicAsset : public Asset
	{
	private:

		sf::Music m_music;

	public:

		bool loadAsset(const std::string &filepath);
		sf::Music &getMusic();
	};
};

#endif