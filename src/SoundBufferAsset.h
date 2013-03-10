#ifndef CC_SOUNDBUFFERASSET_H
#define CC_SOUNDBUFFERASSET_H

#include <SFML/Audio.hpp>
#include "Asset.h"

namespace cc
{
	class SoundBufferAsset : public Asset
	{
	private:

		sf::SoundBuffer m_soundbuffer;

	public:

		bool loadAsset(const std::string &filepath);
		sf::SoundBuffer &getSoundBuffer();
	};
};

#endif