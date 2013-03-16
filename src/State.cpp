#include "State.h"
#include "Engine.h"

namespace jl
{
	State::State(Engine *engine) :
		m_engine(engine),
		m_paused(false)
	{
		m_elapsedClock.reset();
		m_pausedClock.reset();
	}

	void State::pause()
	{
		m_elapsedClock.stop();
		m_pausedClock.start();
		m_paused = true;
	}
	void State::resume()
	{
		m_elapsedClock.start();
		m_pausedClock.stop();
		m_paused = false;
	}
	bool State::isPaused() const
	{
		return m_paused;
	}

	jl::Engine *const State::getEngine() const
	{
		return m_engine;
	}
	
	float State::getElapsedTime() const
	{
		return m_elapsedClock.getElapsedTime().asSeconds();
	}
	float State::getPausedTime() const
	{
		return m_pausedClock.getElapsedTime().asSeconds();
	}

};