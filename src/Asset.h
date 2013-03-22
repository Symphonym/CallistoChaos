#ifndef JL_ASSET_H
#define JL_ASSET_H

#include <string>

namespace jl
{
	// Abstract class for handling Assets
	class Asset
	{
	private:

		friend class AssetManager;

		// This method needs to be overloaded for the asset
		// so that the AssetCache knows how to load this 
		// asset.
		virtual bool loadAsset(const std::string &filepath) = 0;

	public:
	};
};

#endif