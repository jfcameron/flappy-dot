#include <jfc/flappy_event_bus.h>

using namespace flappy;

void event_bus::add_player_wants_to_quit_observer(decltype(m_PlayerWantsToQuit)::observer_weak_ptr_type pObserver)
{
	m_PlayerWantsToQuit.add_observer(pObserver);
}

void event_bus::propagate_player_wants_to_quit_event(decltype(m_PlayerWantsToQuit)::event_type e)
{
	m_PlayerWantsToQuit.propagate_event(e);
}

void event_bus::add_player_wants_to_reset_observer(decltype(m_PlayerWantsToReset)::observer_weak_ptr_type pObserver)
{
	m_PlayerWantsToReset.add_observer(pObserver);
}

void event_bus::propagate_player_wants_to_reset_event(decltype(m_PlayerWantsToReset)::event_type e)
{
	m_PlayerWantsToReset.propagate_event(e);
}

void flappy::event_bus::add_player_count_changed_observer(decltype(m_PlayerCountChanged)::observer_weak_ptr_type pObserver)
{
	m_PlayerCountChanged.add_observer(pObserver);
}

void flappy::event_bus::propagate_player_count_changed_event(decltype(m_PlayerCountChanged)::event_type e)
{
	m_PlayerCountChanged.propagate_event(e);
}

void flappy::event_bus::add_screen_pushed_event_observer(decltype(m_ScreenPushed)::observer_weak_ptr_type pObserver)
{
	m_ScreenPushed.add_observer(pObserver);
}

void flappy::event_bus::propagate_screen_pushed_event(decltype(m_ScreenPushed)::event_type e)
{
	m_ScreenPushed.propagate_event(e);
}

void flappy::event_bus::add_screen_popped_event_observer(decltype(m_ScreenPopped)::observer_weak_ptr_type pObserver)
{
	m_ScreenPopped.add_observer(pObserver);
}

void flappy::event_bus::propagate_screen_popped_event(decltype(m_ScreenPopped)::event_type e)
{
	m_ScreenPopped.propagate_event(e);
}

void flappy::event_bus::add_player_died_event_observer(decltype(m_PlayerDied)::observer_weak_ptr_type pObserver)
{
	m_PlayerDied.add_observer(pObserver);
}

void flappy::event_bus::propagate_player_died_event(decltype(m_PlayerDied)::event_type e)
{
	m_PlayerDied.propagate_event(e);
}
