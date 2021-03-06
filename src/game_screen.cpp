﻿#include <jfc/game_screen.h>

using namespace gdk;

game_screen::game_screen(graphics::context::context_shared_ptr_type pGraphicsContext,
	input::context::context_shared_ptr_type aInputContext,
	audio::context::context_shared_ptr_type aAudio,
	screen_stack_ptr_type aScreens,
	std::shared_ptr<flappy::event_bus> aEventBus,
	flappy::assets::shared_ptr aAssets)
	: m_InputContext(aInputContext)
	, m_Screens(aScreens)
	, m_pGraphicsContext(pGraphicsContext)
	, m_pAudio(aAudio)
	, m_PlayerCountChangedObserver(std::make_shared< std::function<void(flappy::player_count_changed_event)>>(
		[this, aAssets, aEventBus](flappy::player_count_changed_event e)
		{
			m_games.clear();

			for (int i(0); i < e.count; ++i)
				m_games.push_back(std::shared_ptr<flappy::game>(new flappy::game(m_pGraphicsContext,
					m_InputContext,
					m_pAudio,
					m_Screens,
					aEventBus,
					aAssets)));
		}))
	, m_PlayerWantsToResetObserver(std::make_shared<std::function<void(flappy::player_wants_to_reset_event e)>>(
		[this, aAssets, aEventBus](flappy::player_wants_to_reset_event e)
		{
			for (size_t i(0); i < m_games.size(); ++i)
			{
				if (m_games[i].get() == e.game)
				{
					m_games[i].reset(new flappy::game(m_pGraphicsContext,
						m_InputContext,
						m_pAudio,
						m_Screens,
						aEventBus,
						aAssets));
				}
			}

		}))
	, m_PlayerWantsToQuitObserver(std::make_shared<std::function<void(flappy::player_wants_to_quit_event e)>>(
		[this, aAssets, aEventBus](flappy::player_wants_to_quit_event e)
		{
			for (size_t i(0); i < m_games.size(); ++i)
			{
				if (m_games[i].get() == e.game)
				{
					m_games[i].reset(new flappy::game(m_pGraphicsContext,
						m_InputContext,
						m_pAudio,
						m_Screens,
						aEventBus,
						aAssets));
				}
			}
			
			//if top == this pop else no
			m_Screens->pop();
		}))
	, m_pBlackBGScene(gdk::graphics::context::scene_shared_ptr_type(std::move(pGraphicsContext->make_scene())))
	, m_pBlackBGCamera(std::shared_ptr<gdk::camera>(std::move(pGraphicsContext->make_camera())))
{
	m_pBlackBGCamera->set_clear_color({});

	m_pBlackBGScene->add_camera(m_pBlackBGCamera);
	
	(*m_PlayerCountChangedObserver)({ 1 });

	aEventBus->add_player_count_changed_observer(m_PlayerCountChangedObserver);

	aEventBus->add_player_wants_to_reset_observer(m_PlayerWantsToResetObserver);

	aEventBus->add_player_wants_to_quit_observer(m_PlayerWantsToQuitObserver);
}

void game_screen::update(float deltaTime, float aspectRatio, std::pair<int, int> windowSize)
{
    flappy::screen::update(deltaTime, aspectRatio, windowSize);

	struct screen_layout
	{
		std::pair<float, float> winsizeScale;
		std::pair<float, float> topLeft;
		std::pair<float, float> size;
	};

	static const std::vector<std::vector<screen_layout>> layouts({
		// 1 player
		{
			{{1.0f,1.0f}, {0.0f,0.0f},{1.0f,1.0f}},
		},
		// 2 player
		{
			{{1.0f,0.5f}, {0.0f,0.0f},{1.0f,0.5f}},
			{{1.0f,0.5f}, {0.0f,0.5f},{1.0f,0.5f}},
		},
		// 3 player
		{
			{{0.5f,0.5f}, {0.0f,0.0f},{0.5f,0.5f}},
			{{0.5f,0.5f}, {0.5f,0.5f},{0.5f,0.5f}},
			{{0.5f,0.5f}, {0.0f,0.5f},{0.5f,0.5f}},
		},
		// 4 player
		{
			{{0.5f,0.5f}, {0.0f,0.0f},{0.5f,0.5f}},
			{{0.5f,0.5f}, {0.5f,0.0f},{0.5f,0.5f}},
			{{0.5f,0.5f}, {0.0f,0.5f},{0.5f,0.5f}},
			{{0.5f,0.5f}, {0.5f,0.5f},{0.5f,0.5f}},
		},
	});

	m_pBlackBGScene->draw(windowSize);

	std::pair<int, int> size = { 1, 1 };

	auto zeroedPlayerCount = m_games.size() - 1;

	for (decltype(m_games)::size_type i(0); i < m_games.size(); ++i)
	{
		auto ratio =		
			(layouts[zeroedPlayerCount][i].winsizeScale.first * static_cast<float>(windowSize.first))/
			(layouts[zeroedPlayerCount][i].winsizeScale.second * static_cast<float>(windowSize.second));

		m_games[i]->update(deltaTime,
			ratio,
			windowSize,
			layouts[zeroedPlayerCount][i].topLeft,
			layouts[zeroedPlayerCount][i].size);
	}
}
