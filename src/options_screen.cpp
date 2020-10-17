// © 2020 Joseph Cameron - All Rights Reserved

#include <jfc/options_screen.h>

#include <sstream>

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
	m_pMainScene->add_camera(m_pMainCamera);

	//TODO: move this elsewhere. either assets or a new "Flappy::Controls" object or something. 
	m_pControls = gdk::controls::make(aInputContext, true, true, {0});

	// select_binding_pane
	{
		std::set<std::string> binding_names{
			"MenuUp",
			"MenuDown",
			"MenuLeft",
			"MenuRight",
			"Jump",
		};

		float h = 0.0f;

		for (const auto& b : binding_names)
		{
			std::wstringstream ss;

			ss << b.c_str();

			std::wstring wstring = ss.str();

			decltype(m_BindingNamesTexts)::value_type text(new static_text_renderer(aGraphicsContext,
				aAssets->get_textmap(),
				gdk::text_renderer::alignment::center,
				wstring));

			text->set_model_matrix({ 0, h, 0 }, {}, { 0.05f });
			text->add_to_scene(m_pMainScene);
			text->hide();

			h += 0.1f;

			m_BindingNamesTexts.push_back(text);
		}

		select_binding_pane = pane::make_pane();
		{
			select_binding_pane->set_on_just_gained_top([&]()
			{
				std::cout << "select gained top\n";

				for (auto& b : m_BindingNamesTexts) b->show();
			});

			select_binding_pane->set_on_just_lost_top([&]()
			{
				std::cout << "select lost top\n";

				for (auto& b : m_BindingNamesTexts) b->hide();
			});
		}
	}

	// m_main_pane
	{
		const float horizontal = 0.f;
		float height = 0.25f;
		const float height_offset = 0.075f;
		const auto text_alignment = gdk::text_renderer::alignment::center;

		m_MusicText = decltype(m_MusicText)(new dynamic_text_renderer(aGraphicsContext,
			aAssets->get_textmap(),
			text_alignment,
			L"Music ON"));

		m_MusicText->set_model_matrix({ horizontal, height, 0 }, {}, { 0.05f });
		m_MusicText->add_to_scene(m_pMainScene);
		m_MusicText->hide();

		height -= height_offset;

		m_VolumeText = decltype(m_MusicText)(new dynamic_text_renderer(aGraphicsContext,
			aAssets->get_textmap(),
			text_alignment,
			L"Volume 100p"));

		m_VolumeText->set_model_matrix({ horizontal, height, 0 }, {}, { 0.05f });
		m_VolumeText->add_to_scene(m_pMainScene);
		m_VolumeText->hide();

		height -= height_offset;

		m_PlayerConfigSelect = decltype(m_MusicText)(new dynamic_text_renderer(aGraphicsContext,
			aAssets->get_textmap(),
			text_alignment,
			L"Config Player 1"));

		m_PlayerConfigSelect->set_model_matrix({ horizontal, height, 0 }, {}, { 0.05f });
		m_PlayerConfigSelect->add_to_scene(m_pMainScene);
		m_PlayerConfigSelect->hide();

		auto lostFocus = [=]()
		{
			show_current_text();
		};

		m_main_pane = pane::make_pane();
		{
			m_main_pane->set_on_just_gained_top([&]()
			{
				m_MusicText->show();
				m_VolumeText->show();
				m_PlayerConfigSelect->show();
			});

			m_main_pane->set_on_just_lost_top([&]()
			{
				m_MusicText->hide();
				m_VolumeText->hide();	
				m_PlayerConfigSelect->hide();
			});

			auto pMusicButton = m_main_pane->make_element();
			auto pVolumeButton = m_main_pane->make_element();
			auto pPlayerConfigButton = m_main_pane->make_element();

			pMusicButton->set_south_neighbour(pVolumeButton);
			pMusicButton->set_on_just_lost_focus(lostFocus);
			pMusicButton->set_on_just_gained_focus([=]()
			{
				set_current_text(m_MusicText);
			});
			pMusicButton->set_on_activated([=]()
			{
				//change this, obviously. it is just stub
				static bool bState(true);
				bState = !bState;

				if (bState) m_MusicText->update_text(L"Music ON");
				else m_MusicText->update_text(L"Music OFF");
			});

			pVolumeButton->set_north_neighbour(pMusicButton);
			pVolumeButton->set_south_neighbour(pPlayerConfigButton);
			pVolumeButton->set_on_just_lost_focus(lostFocus);
			pVolumeButton->set_on_just_gained_focus([=]()
			{
				set_current_text(m_VolumeText);
			});
			
			pVolumeButton->set_while_focused([=]()
			{
				//change obviously, this is stub.
				static int vol = 100.f;
				const int vol_inc = 10.f;
				if (m_pInput->get_key_just_pressed(keyboard::Key::LeftArrow)) vol -= vol_inc;
				if (m_pInput->get_key_just_pressed(keyboard::Key::RightArrow)) vol += vol_inc;

				vol = vol < 0 ? 0 : vol;
				vol = vol > 100 ? 100 : vol;

				m_VolumeText->update_text(L"Volume " + std::to_wstring(vol));
			});

			pPlayerConfigButton->set_north_neighbour(pVolumeButton);
			pPlayerConfigButton->set_on_just_lost_focus(lostFocus);
			pPlayerConfigButton->set_on_just_gained_focus([=]()
			{
				set_current_text(m_PlayerConfigSelect);
			});
			pPlayerConfigButton->set_while_focused([=]()
			{
				//change obviously, this is stub.
				static const int MAX_PLAYER = 3;

				if (m_pInput->get_key_just_pressed(keyboard::Key::LeftArrow))
				{
					if (m_player_to_configure > 0) m_player_to_configure--;
				}
				if (m_pInput->get_key_just_pressed(keyboard::Key::RightArrow))
				{
					if (m_player_to_configure < MAX_PLAYER) m_player_to_configure++;
				}
				
				m_PlayerConfigSelect->update_text(L"Config Player " + std::to_wstring(1 + m_player_to_configure));
			});
			pPlayerConfigButton->set_on_activated([=]()
			{
				//m_Screens->push(m_GameScreen);
			});
		}
	}

	m_menu->push(m_main_pane);
}

void options_screen::update(float deltaTime, float aspectRatio, std::pair<int, int> windowSize)
{
	flappy::screen::update(deltaTime, aspectRatio, windowSize);

	m_menu->update();

	m_pMainCamera->set_orthographic_projection(2, 2, 0.0075, 10, aspectRatio);
	
	m_pMainScene->draw(windowSize);

	//if (m_pConfig) m_pConfig->update();

	if (m_pInput->get_key_just_pressed(keyboard::Key::Escape))
	{
		m_Screens->pop();
	}
}
