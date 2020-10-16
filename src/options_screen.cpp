// © 2020 Joseph Cameron - All Rights Reserved

#include <jfc/options_screen.h>

using namespace flappy;

options_screen::options_screen(graphics::context::context_shared_ptr_type aGraphicsContext,
	input::context::context_shared_ptr_type aInputContext,
	audio::context::context_shared_ptr_type aAudioContext,
	screen_stack_ptr_type aScreens,
	std::shared_ptr<flappy::event_bus> aEventBus,
	flappy::assets::shared_ptr aAssets)
	: m_pMainScene(graphics::context::scene_shared_ptr_type(std::move(aGraphicsContext->make_scene())))
	, m_pInput(aInputContext)
	, m_pMainCamera(std::shared_ptr<gdk::camera>(std::move(aGraphicsContext->make_camera())))
	, m_Screens(aScreens)
	, m_menu(std::make_shared<decltype(m_menu)::element_type>(gdk::menu(
		[&]() {return m_pInput->get_key_just_pressed(keyboard::Key::UpArrow);},
		[&]() {return m_pInput->get_key_just_pressed(keyboard::Key::DownArrow);},
		[&]() {return m_pInput->get_key_just_pressed(keyboard::Key::LeftArrow);},
		[&]() {return m_pInput->get_key_just_pressed(keyboard::Key::RightArrow);},
		[&]() {return m_pInput->get_key_just_pressed(keyboard::Key::Enter);},
		[&]() {return m_pInput->get_key_just_pressed(keyboard::Key::Escape);})))
	, m_pEventBus(aEventBus)
{
	m_pControls = gdk::controls::make(aInputContext,
		true, true, {0});

	m_pConfig = std::shared_ptr<gdk::configurator>(
		new gdk::configurator(aInputContext, m_pControls, {
			"Menu_Up",
			"Menu_Down",
			"Menu_Left",
			"Menu_Right",
			"Jump"
		}));

	auto blar = m_pConfig->get_binding_names();

	m_pMainScene->add_camera(m_pMainCamera);
}

void options_screen::update(float deltaTime, float aspectRatio, std::pair<int, int> windowSize)
{
	flappy::screen::update(deltaTime, aspectRatio, windowSize);

	m_pMainCamera->set_orthographic_projection(2, 2, 0.0075, 10, aspectRatio);
	
	m_pMainScene->draw(windowSize);

	if (m_pConfig) m_pConfig->update();

	if (m_pInput->get_key_just_pressed(keyboard::Key::Escape))
	{
		m_Screens->pop();
	}
}
