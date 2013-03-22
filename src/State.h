#ifndef JL_STATE_H
#define JL_STATE_H

#include "StopWatch.h"


namespace jl
{
	class Engine;
	class State
	{
	private:

		// Allow only the StateManager to access
		// the private "init" method.
		friend class StateManager;

		jl::Engine *m_engine;
		jl::StopWatch m_elapsedClock, m_pausedClock;
		bool m_paused;

	public:

		virtual ~State(){};

		virtual void events() = 0;
		virtual void update() = 0;
		virtual void render() = 0;

		// "pause" will make the State paused(inactive). Although
		// this won't prevent the State from being updated, it's
		// just to let the user know the State isn't active.
		void pause();
		// "resume" will make the State unpaused(active).
		void resume();

		// "isPaused" returns whether or not the State is paused
		// , active or not.
		bool isPaused() const;

	protected:
		
		explicit State(jl::Engine *engine);

		// "getEngine" returns a pointer to the Engine that 
		// manages the State.
		jl::Engine *const getEngine() const;

		// "getElapsedTime" returns total elapsed time in seconds
		// that the State has been active(unpaused) since the
		// constructor was called.
		float getElapsedTime() const;
		// "getPausedTime" returns total elapsed time in seconds
		// that the State has been inactive(paused) since the
		// constructor was called
		float getPausedTime() const;

	};
};

#endif