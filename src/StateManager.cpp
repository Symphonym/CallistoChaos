#include "StateManager.h"

namespace jl
{

	void StateManager::processDeletes()
	{
		while(!m_delete.empty())
			m_delete.pop_back();
	}


	void StateManager::popState()
	{
		if(!isEmpty())
		{
			// Move ownershipf of State to delete queue
			m_delete.push_back(std::move(m_stack.back()));

			// Ownership is moved, remove stack element
			m_stack.pop_back();
		}

	}
	void StateManager::dropState()
	{
		if(!isEmpty())
		{
			// Move ownership and drop the active State to the bottom
			getActiveState()->pause();
			m_stack.insert(m_stack.begin(), std::move(m_stack.back()));

			// Ownership has been moved, delete stack element
			m_stack.pop_back();
			getActiveState()->resume();
		}
	}

	void StateManager::setState(std::unique_ptr<State> state)
	{
		while(!isEmpty())
			popState();

		pushState(std::move(state));
	}
	void StateManager::pushState(std::unique_ptr<State> state)
	{
		// Initialize the state
		state->init();

		// Pause active state, if there is one
		if(!isEmpty())
			getActiveState()->pause();

		m_stack.push_back(std::move(state));
	}

	jl::State *const StateManager::getActiveState() const
	{
		return m_stack.back().get();
	}
	bool StateManager::isEmpty() const
	{
		return m_stack.empty();
	}
};