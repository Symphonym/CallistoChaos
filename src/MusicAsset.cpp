#include "MusicAsset.h"

namespace jl
{

	bool MusicAsset::loadAsset(const std::string &filepath)
	{
		return m_music.openFromFile(filepath);
	}
	sf::Music &MusicAsset::get()
	{
		return m_music;
	}
};