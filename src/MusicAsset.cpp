#include "MusicAsset.h"

namespace cc
{

	bool MusicAsset::loadAsset(const std::string &filepath)
	{
		return m_music.openFromFile(filepath);
	}
	sf::Music &MusicAsset::getMusic()
	{
		return m_music;
	}
};