// © 2020 Joseph Cameron - All Rights Reserved

#ifndef GDK_EVENT_BUS_H
#define GDK_EVENT_BUS_H

#include <vector>
#include <memory>
#include <functional>

namespace gdk
{
	/// \brief mechanism for producers to push events to a certalized bus, 
/// which are then seen by observers
	template<class event_type_template>
	class event_bus
	{
	public:
		using event_type = event_type_template;

		using observer_type = std::function<void(event_type)>;

		using observer_weak_ptr_type = std::weak_ptr<observer_type>;

		using observer_shared_ptr_type = std::shared_ptr<observer_type>;

	private:
		std::vector<observer_weak_ptr_type> m_Observers;

	public:
		/// \brief notify all observers of the event
		/// remove observers that are null
		void propagate_event(event_type e)
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

		/// \brief adds an observer, to be notified when an event occurs
		void add_observer(observer_weak_ptr_type observer)
		{
			m_Observers.push_back(observer);
		}
	};
}

#endif
