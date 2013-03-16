#ifndef JL_SOUNDBUFFERASSET_H
#define JL_SOUNDBUFFERASSET_H

#include <SFML/Audio.hpp>
#include "Asset.h"

namespace jl
{
	class SoundBufferAsset : public Asset
	{
	private:

		sf::SoundBuffer m_soundbuffer;

	public:

		bool loadAsset(const std::string &filepath);

		// Returns the managed asset
		sf::SoundBuffer &get();
	};
};

#endif