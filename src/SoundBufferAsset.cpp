#include "SoundBufferAsset.h"

namespace jl
{

	bool SoundBufferAsset::loadAsset(const std::string &filepath)
	{
		return m_soundbuffer.loadFromFile(filepath);
	}
	sf::SoundBuffer &SoundBufferAsset::get()
	{
		return m_soundbuffer;
	}
};