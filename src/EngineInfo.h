#ifndef CC_ENGINEINFO_H
#define CC_ENGINEINFO_H

#include <SFML/Graphics.hpp>
#include "Engine.h"

namespace cc
{	
	// As there can only be one Engine active at a time, this class
	// provides Global access to the active Engine
	class EngineInfo
	{
	private:

		friend class Engine;
		static Engine *m_engine;

	public:

		// Getter methods to access data within the Engine

		static sf::Event &getEvent();
		static Renderer &getRender();
		static AssetManager &getAssets();


		static double getDelta();
		static double getFps();

		// Controlling methods communicating with the Engine

		// Controlls the state machine
		static void changeState(std::unique_ptr<State> state);
		static void pushState(std::unique_ptr<State> state);
		static void popState();

		// Controlls rendering drawables onto the screen
		static void render(const sf::Drawable &drawable, const sf::RenderStates &states = sf::RenderStates::Default);

		// Controlls acquiring assets from the AssetManager
		template<typename AssetType> static AssetType* acquire(const std::string &filepath)
		{
			return getAssets().getAsset<AssetType>(filepath);
		};

	};
};

#endif