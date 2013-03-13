#ifndef CC_ASSETCACHE_H
#define CC_ASSETCACHE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <memory>
#include <stdexcept>
#include "Asset.h"

// Include built-in assets for user friendliness
#include "TextureAsset.h"
#include "FontAsset.h"
#include "SoundBufferAsset.h"
#include "MusicAsset.h"

namespace cc
{

	class AssetManager
	{
	private:
		std::map<std::string, std::unique_ptr<Asset>> m_assets;

	public:

		// Returns asset converted to the specified AssetType, attempts to load
		// the asset from the filepath if it could not be found.
		template<typename AssetType> AssetType* getAsset(const std::string &filepath)
		{
			// Check if assets exists, if not; blame the user
			if(m_assets.find(filepath) == m_assets.end())
			{
				// Create new asset
				auto asset = std::unique_ptr<AssetType>(new AssetType());

				// Run asset's load method
				if(!asset->loadAsset(filepath))
					throw std::runtime_error("The asset \"" + filepath + "\" could not be loaded properly.");

				// Store asset
				m_assets[filepath] = std::move(asset);
			}

			return dynamic_cast<AssetType*>(&(*m_assets[filepath]));
		};

		void deleteAsset(const std::string &filepath);

	};
};

#endif