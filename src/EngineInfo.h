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

		static sf::Event &getEvent();
		static Renderer &getRender();
		static AssetManager &getAssets();

		static double getDelta();
		static double getFps();

		static void changeState(std::unique_ptr<State> state);
		static void pushState(std::unique_ptr<State> state);
		static void popState();
	};
};

#endif