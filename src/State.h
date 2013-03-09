#ifndef CC_STATE_H
#define CC_STATE_H

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