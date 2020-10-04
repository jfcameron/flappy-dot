// © 2020 Joseph Cameron - All Rights Reserved

#include <jfc/main_menu_screen.h>

#include <gdk/texture.h>
#include <gdk/text_map.h>

#include <jfc/Text_Sheet.png.h>

#include <sstream>

using namespace gdk;

audio::context::context_shared_ptr_type pAudioContext;

static inline std::wstring playerCountToText(int aCount)
{
	std::wstringstream s;

	s << aCount << L" Player";

	if (aCount > 1) s << L"s";

	return s.str();
}

main_menu_screen::main_menu_screen(graphics::context::context_shared_ptr_type aGraphicsContext,
	input::context::context_shared_ptr_type aInputContext,
	audio::context::context_shared_ptr_type aAudioContext,
	screen_stack_ptr_type aScreens,
	screen_ptr_type aGameScreen,
	std::shared_ptr<glfw_window> aGLFWWindow,
	std::shared_ptr<flappy::event_bus> aEventBus,
	flappy::assets::shared_ptr aAssets)
	: m_pInput(aInputContext)
	, m_Screens(aScreens)
	, m_GameScreen(aGameScreen)
	, m_pMainScene(graphics::context::scene_shared_ptr_type(std::move(aGraphicsContext->make_scene())))
	, m_pMainCamera(std::shared_ptr<gdk::camera>(std::move(aGraphicsContext->make_camera())))
	, scenery(flappy::scenery(aGraphicsContext, aGraphicsContext->get_alpha_cutoff_shader(), m_pMainScene, aAssets))
	, m_menu(std::make_shared<decltype(m_menu)::element_type>(gdk::menu(
		[&]() {return m_pInput->get_key_just_pressed(keyboard::Key::UpArrow);},
		[&]() {return m_pInput->get_key_just_pressed(keyboard::Key::DownArrow);},
		[&]() {return m_pInput->get_key_just_pressed(keyboard::Key::LeftArrow);},
		[&]() {return m_pInput->get_key_just_pressed(keyboard::Key::RightArrow);},
		[&]() {return m_pInput->get_key_just_pressed(keyboard::Key::Enter);},
		[&]() {return m_pInput->get_key_just_pressed(keyboard::Key::Escape);})))
	, m_pEventBus(aEventBus)
{
	pAudioContext = aAudioContext;

	m_pMainScene->add_camera(m_pMainCamera);

	auto map = aAssets->get_textmap();

	m_TitleText = std::make_shared<static_text_renderer>(static_text_renderer(aGraphicsContext,
		map,
		text_renderer::alignment::upper_edge, 
		L"flappy dot"));
	m_TitleText->set_model_matrix({ 0, 0.5f, 0 }, {}, { 0.1f });
	m_TitleText->add_to_scene(m_pMainScene);

	m_VersionText = std::make_shared<static_text_renderer>(static_text_renderer(aGraphicsContext,
		map,
		text_renderer::alignment::right_lower_corner, 
		L"code available at: github.com/jfcameron/gdk-graphics"
	));
	m_VersionText->add_to_scene(m_pMainScene);

	m_PromptText = std::make_shared<static_text_renderer>(static_text_renderer(aGraphicsContext,
		map,
		text_renderer::alignment::center,
		L"space to start"
	));
	m_PromptText->set_model_matrix({ 0, 0.0f, 0 }, {}, { 0.075f });
	m_PromptText->add_to_scene(m_pMainScene);

	m_StartText = std::make_shared<static_text_renderer>(static_text_renderer(aGraphicsContext,
		map,
		text_renderer::alignment::center,
		L"start game"
	));
	m_StartText->set_model_matrix({ 0, 0.15f, 0 }, {}, { 0.05f });
	m_StartText->add_to_scene(m_pMainScene);
	m_StartText->hide();

	m_PlayersCountText = std::make_shared<dynamic_text_renderer>(dynamic_text_renderer(aGraphicsContext,
		map,
		text_renderer::alignment::center,
		playerCountToText(m_PlayerCount)
	));
	m_PlayersCountText->set_model_matrix({ 0, 0.05f, 0 }, {}, { 0.05f });
	m_PlayersCountText->add_to_scene(m_pMainScene);
	m_PlayersCountText->hide();

	m_pCreditsText = std::make_shared<static_text_renderer>(static_text_renderer(aGraphicsContext,
		map,
		text_renderer::alignment::center,
		L"credits"
	));
	m_pCreditsText->set_model_matrix({ 0, -0.05f, 0 }, {}, { 0.05f });
	m_pCreditsText->add_to_scene(m_pMainScene);
	m_pCreditsText->hide();

	m_pQuitText = std::make_shared<static_text_renderer>(static_text_renderer(aGraphicsContext,
		map,
		text_renderer::alignment::center,
		L"quit"
	));
	m_pQuitText->set_model_matrix({ 0, -0.15f, 0 }, {}, { 0.05f });
	m_pQuitText->add_to_scene(m_pMainScene);
	m_pQuitText->hide();

	m_pCreditsContextText = std::make_shared<static_text_renderer>(static_text_renderer(aGraphicsContext,
		map,
		text_renderer::alignment::left_upper_corner,
		L"artwork\n\n"
		L" ld: opengameart.org/users/ld"
		L"\r\r"
		L"Music\n\n"
		" PlayOnLoop:\n opengameart.org/users/playonloop"
		L"\r\r"
		" ProjectsU012:\n freesound.org/people/ProjectsU012/"
		L"\r\r"
		L"code\n\n"
		L" jfcameron:\n github.com/jfcameron/"
	));
	
	m_pCreditsContextText->add_to_scene(m_pMainScene);
	m_pCreditsContextText->hide();

	set_current_text(m_PromptText);
	
	// Credits pane logic
	auto credits_pane = pane::make_pane();
	{
		credits_pane->set_on_just_gained_top([=]()
		{
			m_pCreditsContextText->show();
		});

		credits_pane->set_on_just_lost_top([=]()
		{
			m_pCreditsContextText->hide();
		});
	}

	// Main pane logic
	auto main_pane = pane::make_pane();
	{
		main_pane->set_on_just_gained_top([&]()
		{
			m_StartText->show();
			m_PlayersCountText->show();
			m_pCreditsText->show();
			m_pQuitText->show();
		});

		main_pane->set_on_just_lost_top([&]()
		{
			m_StartText->hide();
			m_PlayersCountText->hide();
			m_pCreditsText->hide();
			m_pQuitText->hide();

			set_current_text(nullptr);
		});

		auto pStartButton = main_pane->make_element();
		auto pPlayersButton = main_pane->make_element();
		auto pCreditsButton = main_pane->make_element();
		auto pQuitButton = main_pane->make_element();

		auto lostFocus = [=]()
		{
			show_current_text();
		};

		pStartButton->set_south_neighbour(pPlayersButton);
		pStartButton->set_on_just_lost_focus(lostFocus);
		pStartButton->set_on_just_gained_focus([=]()
		{
			set_current_text(m_StartText);
		});
		pStartButton->set_on_activated([=]()
		{
			m_Screens->push(m_GameScreen);
		});

		pPlayersButton->set_north_neighbour(pStartButton);
		pPlayersButton->set_south_neighbour(pCreditsButton);
		pPlayersButton->set_on_just_lost_focus(lostFocus);
		pPlayersButton->set_on_just_gained_focus([=]()
		{
			set_current_text(m_PlayersCountText);
		});
		pPlayersButton->set_on_activated([=]()
		{
			if (++m_PlayerCount > 4) m_PlayerCount = 1;

			m_pEventBus->propagate_player_count_changed_event({ m_PlayerCount });

			m_PlayersCountText->update_text(playerCountToText(m_PlayerCount));
		});

		pCreditsButton->set_north_neighbour(pPlayersButton);
		pCreditsButton->set_south_neighbour(pQuitButton);
		pCreditsButton->set_on_just_lost_focus(lostFocus);
		pCreditsButton->set_on_just_gained_focus([=]()
		{
			set_current_text(m_pCreditsText);
		});
		pCreditsButton->set_on_activated([=]()
		{
			m_menu->push(credits_pane);
		});

		pQuitButton->set_north_neighbour(pCreditsButton);
		pQuitButton->set_on_just_lost_focus(lostFocus);
		pQuitButton->set_on_just_gained_focus([=]()
		{
			set_current_text(m_pQuitText);
		});
		pQuitButton->set_on_activated([=]()
		{
			aGLFWWindow->close();
		});
	}

	// Title pane logic
	auto title_pane = pane::make_pane();
	{
		title_pane->set_on_just_lost_top([&]()
		{
			m_PromptText->hide();
		});
		title_pane->set_on_just_gained_top([&]()
		{
			set_current_text(m_PromptText);
		});

		auto pStartPrompt = title_pane->make_element();

		pStartPrompt->set_on_activated([&, main_pane]()
		{
			m_menu->push(main_pane);
		});
	}

	m_menu->push(title_pane);
}

void main_menu_screen::update(float delta, float aspectRatio, std::pair<int, int> windowSize)
{
    flappy::screen::update(delta, aspectRatio, windowSize);

	m_menu->update();
	
	m_VersionText->set_model_matrix({ +0.5f * aspectRatio, -0.5, 0 }, {}, { 0.035 });

	m_pCreditsContextText->set_model_matrix({ -0.5f * aspectRatio, 0.35f, 0 }, {}, { 0.04f });

	m_pMainCamera->set_orthographic_projection(2, 2, 0.0075, 10, aspectRatio);

	m_pMainScene->draw(windowSize);

	scenery.update(delta);
}
