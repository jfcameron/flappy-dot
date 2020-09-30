// © 2020 Joseph Cameron - All Rights Reserved
#include <gdk/event_bus.h>

using namespace gdk;

void event_bus::propagate_event(event_type e)
{
    for (size_t i(0); i < m_Observers.size(); ++i)
    {
        if (auto p = m_Observers[i].lock())
        {
            (*p)(e);
        }
        else
        {
            m_Observers.erase(m_Observers.begin() + i);
        }
    }
}

void event_bus::add_observer(observer_weak_ptr_type observer)
{
    m_Observers.push_back(observer);
}

