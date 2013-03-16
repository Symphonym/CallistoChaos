#ifndef JL_STOPWATCH_H
#define JL_STOPWATCH_H

#include <SFML/System.hpp>

namespace jl
{
	class StopWatch
	{
	private:

		sf::Clock m_clock;
		sf::Time m_elapsedTime;
		bool m_running;

	public:

		void start();
		void stop();

		void restart();
		void reset();

		bool isRunning() const;
		sf::Time getElapsedTime() const;
	};
};

#endif