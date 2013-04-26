#ifndef JL_SOUNDMANAGER_H
#define JL_SOUNDMANAGER_H

#include <SFML/Audio.hpp>
#include <string>

namespace jl
{
	class AssetManager;

	// "SoundManager" provides a global namespace for playing sounds, using the
	// assets stored in the Engine.
	namespace SoundManager
	{
		// "registerAssets" registers the AssetManager from which the
		// SoundManager can acquire it's sf::SoundBuffer objects.
		void registerAssets(AssetManager* assets);

		void addSound(const std::string &filepath);
		void removeSound(const std::string &filepath);

		void playSound(const std::string &filepath);
		void pauseSound(const std::string &filepath);
		void stopSound(const std::string &filepath);

		void clearSounds();

		sf::Sound &getSound(const std::string &filepath);
	};
};

#endif
