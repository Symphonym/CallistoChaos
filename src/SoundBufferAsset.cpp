#include "SoundBufferAsset.h"

namespace cc
{

	bool SoundBufferAsset::loadAsset(const std::string &filepath)
	{
		return m_soundbuffer.loadFromFile(filepath);
	}
	sf::SoundBuffer &SoundBufferAsset::getSoundBuffer()
	{
		return m_soundbuffer;
	}
};