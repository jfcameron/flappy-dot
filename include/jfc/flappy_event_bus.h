#ifndef JFC_FLAPPY_EVENT_BUS_H
#define JFC_FLAPPY_EVENT_BUS_H

#include <gdk/event_bus.h>

namespace flappy
{
	struct screen_pushed_event
	{
		std::string name;
	};

	struct screen_popped_event
	{
		std::string name;
	};

	struct player_died_event
	{
		int score;
	};

	struct player_count_changed_event
	{
		int count;
	};

	/// \brief wrapper for all the event buses used in the program.
    class event_bus final
    {
	public:

	private:
		gdk::event_bus<screen_pushed_event> m_ScreenPushed;
		gdk::event_bus<screen_popped_event> m_ScreenPopped;
		
		gdk::event_bus<player_count_changed_event> m_PlayerCountChanged;

		gdk::event_bus<player_died_event> m_PlayerDied;

    public:
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
