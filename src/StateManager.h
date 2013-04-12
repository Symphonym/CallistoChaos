#ifndef JL_STATEMANAGER_H
#define JL_STATEMANAGER_H

#include <vector>
#include <memory>
#include <string>
#include "State.h"

namespace jl
{
	class StateManager
	{
	private:

		friend class Engine;

		// The State stack, and the delete requests
		std::vector<std::unique_ptr<jl::State>> m_stack, m_delete;

		// "processDeletes" loops through all delete requests, processing
		// them. Only the Engine will run this method.
		void processDeletes();

	public:

		// "popState" requests the active State to be popped
		// from the top of the stack, effectively deleting it.
		void popState();
		// "dropState" requests the active State to be dropped
		// to the absolute bottom of the stack, making it inactive.
		void dropState();

		// "setState" pops all States on the stack and sets the
		// active State to the specified State.
		void setState(std::unique_ptr<State> state);
		// "pushState" pushes the specified State to the top of
		// the State stack, making it the active State.
		void pushState(std::unique_ptr<State> state);

		// "clearStack" clears all states from the stack, effectively
		// exiting the game.
		void clearStack();

		// "getActiveState" returns the active State of the stack,
		// which will always be the State on the top of the stack.
		jl::State *const getActiveState() const;
		// "isEmpty" returns whether or not the stack is empty
		bool isEmpty() const;

	};
};

#endif