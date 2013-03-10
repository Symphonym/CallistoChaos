#include "AssetManager.h"

namespace cc
{
	void AssetManager::deleteAsset(const std::string& filepath)
	{
		// Attemps to delete the asset if it exists
		if(m_assets.find(filepath) != m_assets.end())
			m_assets.erase(m_assets.find(filepath));
	}
};