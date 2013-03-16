#include "StopWatch.h"

namespace jl
{

	void StopWatch::start()
	{
		if(!m_running)
		{
			m_clock.restart();
			m_running = true;
		}
	}
	void StopWatch::stop()
	{
		if(m_running)
		{
			m_elapsedTime += m_clock.getElapsedTime();
			m_running = false;
		}
	}
	void StopWatch::restart()
	{
		reset();
		start();
	}
	void StopWatch::reset()
	{
		m_elapsedTime = sf::Time::Zero;;
		m_running = false;
		m_clock.restart();
	}

	bool StopWatch::isRunning() const
	{
		return m_running;
	}
	sf::Time StopWatch::getElapsedTime() const
	{
		if(m_running)
			return m_elapsedTime + m_clock.getElapsedTime();
		else
			return m_elapsedTime;
	}
};