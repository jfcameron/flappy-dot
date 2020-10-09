// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_STATE_MACHINE_H
#define JFC_STATE_MACHINE_H

#include<functional>
#include<memory>
#include<vector>

namespace jfc
{
	/// \brief state is stored in the provided enum type
	///
	/// observers are notified when state changes, at which time
	/// they will see both the old and new states
    template<class enum_type_template>
    class state_machine final
    {
		static_assert(std::is_enum<enum_type_template>::value);

    public:
		/// \brief state type
        using enum_type = enum_type_template;

		/// \brief state change observer type
        using observer_type = std::function<void(enum_type, enum_type)>;

		/// \brief ptr to observer
        using observer_ptr = std::weak_ptr<observer_type>;
   
    private:
		/// \brief internal state
        enum_type m_state;

		/// \brief collection of observers
        std::vector<observer_ptr> m_observers;

    public:
		/// \brief gets internal state
		[[nodiscard]] enum_type get() const noexcept
		{
			return m_state;
		}

        /// \brief adds an observer to state changes
        void add_observer(observer_ptr p)
        {
            m_observers.push_back(p);
        }

		/// \brief change state, notify observers
        void set(enum_type a)
        {
			for (size_t i(0); i < m_observers.size(); ++i)
            {
                if (auto p = m_observers[i].lock()) (*p)(m_state, a);
                else m_observers.erase(m_observers.begin() + i);
            }

			m_state = a;
        }

		/// \brief enable copy semantics
		state_machine& operator=(const state_machine&) = default;
		/// \brief enable copy semantics
		state_machine(const state_machine&) = default;

		/// \brief enable move semantics
		state_machine& operator=(state_machine&&) = default;
		/// \brief enable move semantics
		state_machine(state_machine&&) = default;

		/// \brief ctor. enum param determines initial state of the state machine
        state_machine(enum_type initialState)
        : m_state(initialState)
        {}

		/// \brief dtor
		~state_machine() = default;
    };
}

#endif
