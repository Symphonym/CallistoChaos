#ifndef CC_SOUNDMANAGER_H
#define CC_SOUNDMANAGER_H

#include <SFML/Audio.hpp>
#include <string>

namespace cc
{
	namespace SoundManager
	{
		void addSound(const std::string &filepath);
		void removeSound(const std::string &filepath);

		void playSound(const std::string &filepath);
		void pauseSound(const std::string &filepath);
		void stopSound(const std::string &filepath);

		sf::Sound &getSound(const std::string &filepath);
	};
};

#endif