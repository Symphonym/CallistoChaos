#include "StateManager.h"

namespace jl
{

	void StateManager::processDeletes()
	{
		for(int i = 0; i < m_delete.size(); i++)
		{
			m_stack.erase(m_stack.begin() + m_delete[i]);
			m_delete.pop_back();
		}
	}


	void StateManager::popState()
	{
		if(!isEmpty())
		{
			// Move ownershipf of State to delete queue
			m_delete.push_back(m_stack.size()-1);
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
		clearStack();

		pushState(std::move(state));
	}
	void StateManager::pushState(std::unique_ptr<State> state)
	{
		// Pause active state, if there is one
		if(!isEmpty())
			getActiveState()->pause();

		m_stack.push_back(std::move(state));
	}

	void StateManager::clearStack()
	{
		for(int i = 0; i < m_stack.size(); i++)
		{
			m_delete.push_back(i);
		}
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
