// � 2020 Joseph Cameron - All Rights Reserved

#ifndef SCREEN_STACK_H
#define SCREEN_STACK_H

#include <stack>
#include <memory>

#include <gdk/screen.h>

namespace gdk
{
	// TODO: replace "push" and "pop" functors with gained_top lost_top
	class screen_stack
	{
	public:
		using stack_type = std::stack<std::shared_ptr<gdk::screen>>;
		using push_pop_functor_type = std::function<void(stack_type::value_type)>;

	private:
		//! internal stack type
		stack_type m_Screens;

		//! called when a screen is pushed to the stack
		push_pop_functor_type m_PushFunctor;

		//! called when a screen is popped from the stack
		push_pop_functor_type m_PopFunctor;

	public:
		void push(std::shared_ptr<gdk::screen> s)
		{
			m_Screens.push(s);

			m_PushFunctor(s);
		}

		//! pops the screen stack
		/// \remark has no effect if the stack would completely empty
		void pop()
		{
			if (m_Screens.size() > 1)
			{
				m_Screens.pop();

				// TODO: This should be revisted this seems weird
				if (!m_Screens.empty()) m_PopFunctor(m_Screens.top());
			}
		}

		void update(float delta, float windowAspectRatio, std::pair<int, int> windowSize)
		{
			if (m_Screens.size()) m_Screens.top()->update(delta, windowAspectRatio, windowSize);
		}

		//! move semantics
		screen_stack(screen_stack&&) = default;
		//! move semantics
		screen_stack &operator=(screen_stack&&) = default;

		screen_stack(push_pop_functor_type aOnPushedFunctor, push_pop_functor_type aOnPoppedFunctor) 
			: m_PushFunctor(aOnPushedFunctor)
			, m_PopFunctor(aOnPoppedFunctor)
		{}
	};
}

using screen_ptr_type = std::shared_ptr<gdk::screen>;
using screen_stack_type = gdk::screen_stack;
using screen_stack_ptr_type = std::shared_ptr<screen_stack_type>;

#endif
