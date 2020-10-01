// © 2020 Joseph Cameron - All Rights Reserved

#ifndef GDK_MENU_H
#define GDK_MENU_H

#include <gdk/vector2.h>

#include <functional>
#include <stack>

namespace gdk
{
	class element
	{
	public:
		using neighbour_weakptr_type = std::weak_ptr<element>;

		using on_focused_type = std::function<void()>;

		using on_activated_type = std::function<void()>;

		//using on_touch_hover_type
		//^ if cursor is in box or whatever, sets current element to this

		//using on_touch_pressed_type = std::function<void()>; //NOT USED YET. NEED TO IMPLEMENT TOUCH
		//^ calls a dedicated callback, so user can have special depressed button graphic or whatever.

		//using on_touch_released_type = std::function<void()>; // NEED TOUCH
		//^ calls on_activated

	public:
		neighbour_weakptr_type m_north_neighbour;

		neighbour_weakptr_type m_east_neighbour;

		neighbour_weakptr_type m_south_neighbour;

		neighbour_weakptr_type m_west_neighbour;

		on_activated_type m_activated_functor = []() {};

		on_focused_type m_just_gained_focus = []() {};

		on_focused_type m_just_lost_focus = []() {};

	public:
		void set_on_activated(const decltype(m_activated_functor)& a);

		void set_on_just_gained_focus(const decltype(m_just_gained_focus)& a);

		void set_on_just_lost_focus(const decltype(m_just_lost_focus)& a);


		void set_north_neighbour(neighbour_weakptr_type p);

		decltype(m_north_neighbour) get_north_neighbour() const;

		void set_south_neighbour(neighbour_weakptr_type p);

		decltype(m_south_neighbour) get_south_neighbour() const;

		void set_east_neighbour(neighbour_weakptr_type p);

		decltype(m_east_neighbour) get_east_neighbour() const;

		void set_west_neighbour(neighbour_weakptr_type p);

		decltype(m_west_neighbour) get_west_neighbour() const;
	};

    class pane
    {
	public:
		using pane_shared_ptr = std::shared_ptr<pane>;

		using element_shared_ptr = std::shared_ptr<element>;

		using element_collection_type = std::vector<element_shared_ptr>;

		using on_update_type = std::function<void()>;

	private:
		element_collection_type m_elements;

		element_shared_ptr m_current_element;

	protected:
		void set_current_element(decltype(m_current_element) e);

		decltype(m_current_element) get_current_element();

	public:
		static pane::pane_shared_ptr make_pane();

		virtual void set_on_update(on_update_type a) = 0;

		virtual void set_on_just_gained_top(on_update_type a) = 0;

		virtual void set_on_just_lost_top(on_update_type a) = 0;

		element_shared_ptr make_element();

	protected:
		pane() = default;

	public:
		virtual ~pane() = default;
    };

	class menu;

    class pane_impl : public pane
    {
	public:
		using menu_ptr = menu *;
		
	private:
		menu_ptr m_menu = nullptr;

		on_update_type m_on_update = []() {};

		on_update_type m_on_just_gained_top = []() {};

		on_update_type m_on_just_lost_top = []() {};

	public:
		virtual void set_on_update(on_update_type a) override;

		virtual void set_on_just_gained_top(on_update_type a) override;

		virtual void set_on_just_lost_top(on_update_type a) override;

		void set_menu_ptr(decltype(m_menu) p);

		void update(bool aUpInput,
			bool aDownInput,
			bool aLeftInput,
			bool aRightInput,
			bool aSelectInput,
			bool aCancelInput);

		void just_gained_top();

		void just_lost_top();
		
		~pane_impl() = default;
    };

	class menu
    {
	public:
		using input_functor_type = std::function<bool()>;

	private:
        std::stack<pane::pane_shared_ptr> m_stack;

        input_functor_type m_UpInput;
        input_functor_type m_DownInput;
        input_functor_type m_LeftInput;
        input_functor_type m_RightInput;
        input_functor_type m_Select;
        input_functor_type m_Cancel;

	public:
		/// \brief pushes a pane to the stack
		void push(pane::pane_shared_ptr p);

		/// \brief pops the stack IF at least 1 pane would be left in it
		void pop();

		/// \brief call this in your game loop
		void update();

		/// \brief the provided functors tell the menu about the state of user input
		/// the menu makes no assumptions about controls, so its up to the user to provide
		/// these functors to adapt their input system to the menu system
		menu(input_functor_type aUpInput,
			input_functor_type aDownInput,
			input_functor_type aLeftInput,
			input_functor_type aRightInput,
			input_functor_type aSelect,
			input_functor_type aCancel);
    };
}

#endif
