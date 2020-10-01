// © 2020 Joseph Cameron - All Rights Reserved
#include <gdk/menu.h>

using namespace gdk;

void element::set_on_activated(const on_activated_type& a)
{
    m_activated_functor = a;
}

void element::set_on_just_gained_focus(const decltype(m_just_gained_focus)& a)
{
	m_just_gained_focus = a;
}

void element::set_on_just_lost_focus(const decltype(m_just_lost_focus)& a)
{
	m_just_lost_focus = a;
}

void element::set_north_neighbour(neighbour_weakptr_type p)
{
    m_north_neighbour = p;
}

decltype(element::m_north_neighbour) element::get_north_neighbour() const
{
    return m_north_neighbour;
}

void element::set_south_neighbour(neighbour_weakptr_type p)
{
    m_south_neighbour = p;
}

decltype(element::m_south_neighbour) element::get_south_neighbour() const
{
    return m_south_neighbour;
}

void element::set_east_neighbour(neighbour_weakptr_type p)
{
    m_east_neighbour = p;
}

decltype(element::m_east_neighbour) element::get_east_neighbour() const
{
    return m_east_neighbour;
}

void element::set_west_neighbour(neighbour_weakptr_type p)
{
    m_west_neighbour = p;
}

decltype(element::m_west_neighbour) element::get_west_neighbour() const
{
    return m_west_neighbour;
}

void pane_impl::set_on_update(on_update_type a)
{
	m_on_update = a;
}

pane::element_shared_ptr pane::make_element()
{
	auto p = std::make_shared<element_shared_ptr::element_type>(element());

	if (!m_elements.size()) m_current_element = p;

	m_elements.push_back(p);

	return p;
}

void pane::set_current_element(decltype(m_current_element) e)
{
	if (m_current_element) m_current_element->m_just_lost_focus();

	m_current_element = e;

	m_current_element->m_just_gained_focus();
}

decltype(pane::m_current_element) pane::get_current_element()
{
	return m_current_element;
}

void pane_impl::set_menu_ptr(decltype(pane_impl::m_menu) p)
{
	m_menu = p;
}

void pane_impl::update(bool aUpInput,
	bool aDownInput,
	bool aLeftInput,
	bool aRightInput,
	bool aSelectInput,
	bool aCancelInput)
{
	m_on_update();

	if (aCancelInput)
	{
		if (m_menu) m_menu->pop();
	}

	if (auto currentElement = get_current_element())
	{
		if (aSelectInput)
		{
			currentElement->m_activated_functor();
		}

		if (static_cast<int>(aUpInput) + static_cast<int>(aDownInput) == 1) 
		{
			if (aUpInput)
			{
				if (auto p = currentElement->get_north_neighbour().lock())
					set_current_element(p);
			}
			else
			{
				if (auto p = currentElement->get_south_neighbour().lock())
					set_current_element(p);
			}
		}
		
		if (static_cast<int>(aLeftInput) + static_cast<int>(aRightInput) == 1)
		{
			if (aLeftInput)
			{
				if (auto p = currentElement->get_west_neighbour().lock())
					set_current_element(p);
			}
			else
			{
				if (auto p = currentElement->get_east_neighbour().lock())
					set_current_element(p);
			}
		}
	}
}

pane::pane_shared_ptr pane::make_pane()
{
	return pane::pane_shared_ptr(new pane_impl());
}

void pane_impl::set_on_just_gained_top(pane::on_update_type a)
{
	m_on_just_gained_top = a;
}

void pane_impl::set_on_just_lost_top(pane::on_update_type a)
{
	m_on_just_lost_top = a;
}

void pane_impl::just_gained_top()
{
	m_on_just_gained_top();

	if (auto p = get_current_element()) p->m_just_gained_focus();
}

void pane_impl::just_lost_top()
{
	if (auto p = get_current_element()) p->m_just_lost_focus();

	m_on_just_lost_top();
}

void menu::push(pane::pane_shared_ptr p)
{
	auto pNewTop = std::static_pointer_cast<pane_impl>(p);
	pNewTop->just_gained_top();
	pNewTop->set_menu_ptr(this);

	if (m_stack.size()) 
		std::static_pointer_cast<pane_impl>(m_stack.top())->just_lost_top();

	m_stack.push(p);
}

void menu::pop()
{
	if (m_stack.size() > 1)
	{
		auto pOldTop = std::static_pointer_cast<pane_impl>(m_stack.top());
		pOldTop->just_lost_top();
		pOldTop->set_menu_ptr(nullptr);

		m_stack.pop();
	}

	if (m_stack.size()) std::static_pointer_cast<pane_impl>(m_stack.top())->just_gained_top();
}

void menu::update()
{
	if (m_stack.size())
	{
		std::static_pointer_cast<pane_impl>(m_stack.top())->
			update(m_UpInput(),
				m_DownInput(),
				m_LeftInput(),
				m_RightInput(),
				m_Select(),
				m_Cancel());
	}
}

menu::menu(input_functor_type aUpInput,
	input_functor_type aDownInput,
	input_functor_type aLeftInput,
	input_functor_type aRightInput,
	input_functor_type aSelect,
	input_functor_type aCancel)
	: m_UpInput(aUpInput)
	, m_DownInput(aDownInput)
	, m_LeftInput(aLeftInput)
	, m_RightInput(aRightInput)
	, m_Select(aSelect)
	, m_Cancel(aCancel)
{}
