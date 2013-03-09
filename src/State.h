#ifndef STATE_H
#define STATE_H

namespace cc
{
	class State
	{
	public:

		virtual void init();
		virtual void events();
		virtual void update();
		virtual void render();
	};
};

#endif