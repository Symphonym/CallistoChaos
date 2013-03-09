#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "Engine.h"

namespace cc
{
	class GameEngine : public Engine
	{
	public:

		void events(){};
		void update(){};
		void render(){};
	};
};

#endif