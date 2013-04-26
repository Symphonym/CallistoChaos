#include "AssetManager.h"

namespace jl
{
	sf::Texture &AssetManager::getTexture(const std::string &filepath)
	{
		// Check if texture exists, if not; blame the user
		if(m_textures.find(filepath) == m_textures.end())
		{
			// Create new texture
			sf::Texture texture = sf::Texture();

			// Load texture
			if(!texture.loadFromFile(filepath))
				throw std::runtime_error("The texture \"" + filepath + "\" could not be loaded properly.");

			// Store texture
			m_textures[filepath] = texture;
		}

		return m_textures[filepath];
	}
	sf::Font &AssetManager::getFont(const std::string &filepath)
	{
		// Check if font exists, if not; blame the user
		if(m_fonts.find(filepath) == m_fonts.end())
		{
			// Create new font
			sf::Font font = sf::Font();

			// Load font
			if(!font.loadFromFile(filepath))
				throw std::runtime_error("The font \"" + filepath + "\" could not be loaded properly.");

			// Store font
			m_fonts[filepath] = font;
		}

		return m_fonts[filepath];
	}
	sf::SoundBuffer &AssetManager::getSound(const std::string &filepath)
	{
		// Check if sound exists, if not; blame the user
		if(m_sounds.find(filepath) == m_sounds.end())
		{
			// Create new sound
			sf::SoundBuffer sound = sf::SoundBuffer();

			// Load sound
			if(!sound.loadFromFile(filepath))
				throw std::runtime_error("The sound \"" + filepath + "\" could not be loaded properly.");

			// Store sound
			m_sounds[filepath] = sound;
		}

		return m_sounds[filepath];
	}
	sf::Music &AssetManager::getMusic(const std::string &filepath)
	{
		// Check if music exists, if not; blame the user
		if(m_music.find(filepath) == m_music.end())
		{
			// Create new music
			std::unique_ptr<sf::Music> music = std::unique_ptr<sf::Music>(new sf::Music());

			// Load music
			if(!music->openFromFile(filepath))
				throw std::runtime_error("The music \"" + filepath + "\" could not be loaded properly.");

			// Store music
			m_music[filepath] = std::move(music);
		}

		return *m_music[filepath];
	}

	void AssetManager::deleteTexture(const std::string& filepath)
	{
		// Attemps to delete the texture if it exists
		if(m_textures.find(filepath) != m_textures.end())
			m_textures.erase(m_textures.find(filepath));
	}
	void AssetManager::deleteFont(const std::string& filepath)
	{
		// Attemps to delete the font if it exists
		if(m_fonts.find(filepath) != m_fonts.end())
			m_fonts.erase(m_fonts.find(filepath));
	}
	void AssetManager::deleteSound(const std::string& filepath)
	{
		// Attemps to delete the sound if it exists
		if(m_sounds.find(filepath) != m_sounds.end())
			m_sounds.erase(m_sounds.find(filepath));
	}
	void AssetManager::deleteMusic(const std::string& filepath)
	{
		// Attemps to delete the music if it exists
		if(m_music.find(filepath) != m_music.end())
			m_music.erase(m_music.find(filepath));
	}
};