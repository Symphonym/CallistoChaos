#include "SoundManager.h"
#include "AssetManager.h"
#include <map>

namespace jl
{
	namespace SoundManager
	{
		std::map<std::string, sf::Sound> m_sounds = std::map<std::string, sf::Sound>();
		AssetManager* m_assets = nullptr;

		void registerAssets(AssetManager* assets)
		{
			m_assets = assets;
		}

		void addSound(const std::string &filepath)
		{
			if(m_sounds.find(filepath) == m_sounds.end())
				m_sounds[filepath] = sf::Sound(m_assets->getSound(filepath));;

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

		void clearSounds()
		{
			m_sounds.clear();
		}


		sf::Sound &getSound(const std::string &filepath)
		{
			return m_sounds[filepath];
		}
	};
};
