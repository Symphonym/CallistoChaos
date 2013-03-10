#include "SoundManager.h"
#include "EngineInfo.h"
#include <map>

namespace cc
{
	namespace SoundManager
	{
		std::map<std::string, sf::Sound> m_sounds;

		void addSound(const std::string &filepath)
		{
			sf::Sound sound(EngineInfo::getAssets().getAsset<SoundBufferAsset>(filepath)->get());
			m_sounds[filepath] = sound;
		}
		void removeSound(const std::string &filepath)
		{
			if(m_sounds.find(filepath) != m_sounds.end())
				m_sounds.erase(m_sounds.find(filepath));
		}

		void playSound(const std::string &filepath)
		{
			m_sounds[filepath].play();
		}
		void pauseSound(const std::string &filepath)
		{
			m_sounds[filepath].pause();
		}
		void stopSound(const std::string &filepath)
		{
			m_sounds[filepath].stop();
		}

		sf::Sound &getSound(const std::string &filepath)
		{
			return m_sounds[filepath];
		}
	};
};