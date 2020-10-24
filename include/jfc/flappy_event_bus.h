#ifndef JFC_FLAPPY_EVENT_BUS_H
#define JFC_FLAPPY_EVENT_BUS_H

#include <string>

#include <jfc/event_bus.h>

#include <gdk/screen_stack.h>

namespace flappy
{
	class game;

	struct screen_pushed_event
	{
		//TODO: remove this, just rely on the ptr
		std::string name;

		screen_ptr_type screen;
	};

	struct screen_popped_event
	{
		std::string name;
	};

	struct player_died_event
	{
		size_t score;
	};

	struct player_count_changed_event
	{
		int count;
	};

	struct player_wants_to_reset_event
	{
		/// \warn strictly for comparisons!
		flappy::game *const game;
	};

	struct player_wants_to_quit_event
	{
		/// \warn strictly for comparisons!
		flappy::game* const game;
	};

	/// \brief wrapper for all the event buses used in the program.
    class event_bus final
    {
	public:

	private:
		/// \brief notifies observers that a screen has been pushed
		jfc::event_bus<screen_pushed_event> m_ScreenPushed;
		jfc::event_bus<screen_popped_event> m_ScreenPopped;
		
		jfc::event_bus<player_count_changed_event> m_PlayerCountChanged;
		jfc::event_bus<player_died_event> m_PlayerDied;
		jfc::event_bus<player_wants_to_reset_event> m_PlayerWantsToReset;
		jfc::event_bus<player_wants_to_quit_event> m_PlayerWantsToQuit;

    public:
		void add_player_wants_to_quit_observer(decltype(m_PlayerWantsToQuit)::observer_weak_ptr_type pObserver);
		void propagate_player_wants_to_quit_event(decltype(m_PlayerWantsToQuit)::event_type e);

		void add_player_wants_to_reset_observer(decltype(m_PlayerWantsToReset)::observer_weak_ptr_type pObserver);
		void propagate_player_wants_to_reset_event(decltype(m_PlayerWantsToReset)::event_type e);

		void add_player_count_changed_observer(decltype(m_PlayerCountChanged)::observer_weak_ptr_type pObserver);
		void propagate_player_count_changed_event(decltype(m_PlayerCountChanged)::event_type e);

		void add_screen_pushed_event_observer(decltype(m_ScreenPushed)::observer_weak_ptr_type pObserver);
		void propagate_screen_pushed_event(decltype(m_ScreenPushed)::event_type e);

		void add_screen_popped_event_observer(decltype(m_ScreenPopped)::observer_weak_ptr_type pObserver);
		void propagate_screen_popped_event(decltype(m_ScreenPopped)::event_type e);
    
		void add_player_died_event_observer(decltype(m_PlayerDied)::observer_weak_ptr_type pObserver);
		void propagate_player_died_event(decltype(m_PlayerDied)::event_type e);
	};
}

#endif
