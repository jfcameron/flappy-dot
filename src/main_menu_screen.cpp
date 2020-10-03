#include <jfc/main_menu_screen.h>

#include <gdk/texture.h>
#include <gdk/text_map.h>

#include <jfc/Text_Sheet.png.h>

#include <sstream>

using namespace gdk;

static constexpr int BLINK_RATE(26);

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
	std::shared_ptr<flappy::event_bus> aEventBus)
	: m_pInput(aInputContext)
	, m_Screens(aScreens)
	, m_GameScreen(aGameScreen)
	, m_pMainScene(graphics::context::scene_shared_ptr_type(std::move(aGraphicsContext->make_scene())))
	, m_pMainCamera(std::shared_ptr<gdk::camera>(std::move(aGraphicsContext->make_camera())))
	, scenery(flappy::scenery(aGraphicsContext, aGraphicsContext->get_alpha_cutoff_shader(), m_pMainScene))
	, m_menu(std::make_shared<decltype(m_menu)::element_type>(gdk::menu(
		[&]() {return m_pInput->get_key_just_pressed(keyboard::Key::UpArrow);},
		[&]() {return m_pInput->get_key_just_pressed(keyboard::Key::DownArrow);},
		[&]() {return m_pInput->get_key_just_pressed(keyboard::Key::LeftArrow);},
		[&]() {return m_pInput->get_key_just_pressed(keyboard::Key::RightArrow);},
		[&]() {return m_pInput->get_key_just_pressed(keyboard::Key::A);},
		[&]() {return m_pInput->get_key_just_pressed(keyboard::Key::S);})))
	, m_pEventBus(aEventBus)
{
	pAudioContext = aAudioContext;

	m_pMainScene->add_camera(m_pMainCamera);

	auto pTextTexture = std::shared_ptr<gdk::texture>(std::shared_ptr<texture>(std::move(aGraphicsContext->make_texture(
		{ Text_Sheet_png, Text_Sheet_png + sizeof Text_Sheet_png / sizeof Text_Sheet_png[0] }))));

	text_map map(pTextTexture, { 8, 8 },
	{
		{'a', {0,0}},
		{'b', {1,0}},
		{'c', {2,0}},
		{'d', {3,0}},
		{'e', {4,0}},
		{'f', {5,0}},
		{'g', {6,0}},
		{'h', {7,0}},
		{'i', {0,1}},
		{'j', {1,1}},
		{'k', {2,1}},
		{'l', {3,1}},
		{'m', {4,1}},
		{'n', {5,1}},
		{'o', {6,1}},
		{'p', {7,1}},
		{'q', {0,2}},
		{'r', {1,2}},
		{'s', {2,2}},
		{'t', {3,2}},
		{'u', {4,2}},
		{'v', {5,2}},
		{'w', {6,2}},
		{'x', {7,2}},
		{'y', {0,3}},
		{'z', {1,3}},
		{'A', {0,0}},
		{'B', {1,0}},
		{'C', {2,0}},
		{'D', {3,0}},
		{'E', {4,0}},
		{'F', {5,0}},
		{'G', {6,0}},
		{'H', {7,0}},
		{'I', {0,1}},
		{'J', {1,1}},
		{'K', {2,1}},
		{'L', {3,1}},
		{'M', {4,1}},
		{'N', {5,1}},
		{'O', {6,1}},
		{'P', {7,1}},
		{'Q', {0,2}},
		{'R', {1,2}},
		{'S', {2,2}},
		{'T', {3,2}},
		{'U', {4,2}},
		{'V', {5,2}},
		{'W', {6,2}},
		{'X', {7,2}},
		{'Y', {0,3}},
		{'Z', {1,3}},
		{'0', {3,3}},
		{'1', {4,3}},
		{'2', {5,3}},
		{'3', {6,3}},
		{'4', {7,3}},
		{'5', {0,4}},
		{'6', {1,4}},
		{'7', {2,4}},
		{'8', {3,4}},
		{'9', {4,4}},
		{'!', {2,3}},
		{'.', {5,4}},
		{':', {6,4}},
		{'?', {7,3}},
		{' ', {7,6}},
		{'/', {0,5}},
		{'-', {1,5}},
	});

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

	m_pCurrentText = m_PromptText;
	
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
		static constexpr int RESET_VALUE(BLINK_RATE/2);

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

			m_pCurrentText = nullptr;
		});

		auto pStartButton = main_pane->make_element();
		auto pPlayersButton = main_pane->make_element();
		auto pCreditsButton = main_pane->make_element();
		auto pQuitButton = main_pane->make_element();

		auto lostFocus = [=]()
		{
			if (m_pCurrentText) m_pCurrentText->show();
		};

		pStartButton->set_south_neighbour(pPlayersButton);
		pStartButton->set_on_just_lost_focus(lostFocus);
		pStartButton->set_on_just_gained_focus([=]()
		{
			m_pCurrentText = m_StartText;
			m_pCurrentText->hide();
			m_PrompCounter = RESET_VALUE;
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
			m_pCurrentText = m_PlayersCountText;
			m_pCurrentText->hide();
			m_PrompCounter = RESET_VALUE;
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
			m_pCurrentText = m_pCreditsText;
			m_pCurrentText->hide();
			m_PrompCounter = RESET_VALUE;
		});
		pCreditsButton->set_on_activated([=]()
		{
			m_menu->push(credits_pane);
		});

		pQuitButton->set_north_neighbour(pCreditsButton);
		pQuitButton->set_on_just_lost_focus(lostFocus);
		pQuitButton->set_on_just_gained_focus([=]()
		{
			m_pCurrentText = m_pQuitText;
			m_pCurrentText->hide();
			m_PrompCounter = RESET_VALUE;
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
			m_pCurrentText = m_PromptText;
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
	m_menu->update();

	if (++m_PrompCounter % BLINK_RATE == 0)
	{
		m_BlinkStatus = !m_BlinkStatus;

		if (m_pCurrentText)
		{
			if (m_BlinkStatus) m_pCurrentText->show();
			else m_pCurrentText->hide();
		}
	}
	
	m_VersionText->set_model_matrix({ +0.5f * aspectRatio, -0.5, 0 }, {}, { 0.035 });

	m_pCreditsContextText->set_model_matrix({ -0.5f * aspectRatio, 0.35f, 0 }, {}, { 0.04f });

	m_pMainCamera->set_orthographic_projection(2, 2, 0.0075, 10, aspectRatio);

	m_pMainScene->draw(windowSize);

	scenery.update(delta);
}
