#ifndef JL_ASSETCACHE_H
#define JL_ASSETCACHE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <memory>
#include <stdexcept>

namespace jl
{

	class AssetManager
	{
	private:
		std::map<std::string, sf::Texture> m_textures;
		std::map<std::string, sf::Font> m_fonts;
		std::map<std::string, sf::SoundBuffer> m_sounds;
		std::map<std::string, std::unique_ptr<sf::Music>> m_music;

	public:

		sf::Texture &getTexture(const std::string &filepath);
		sf::Font &getFont(const std::string &filepath);
		sf::SoundBuffer &getSound(const std::string &filepath);
		sf::Music &getMusic(const std::string &filepath);

		void deleteTexture(const std::string &filepath);
		void deleteFont(const std::string &filepath);
		void deleteSound(const std::string &filepath);
		void deleteMusic(const std::string &filepath);

	};
};

#endif