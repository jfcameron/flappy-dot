#include <jfc/game_screen.h>

using namespace gdk;

game_screen::game_screen(graphics::context::context_shared_ptr_type pGraphicsContext,
	input::context::context_shared_ptr_type aInputContext,
	audio::context::context_shared_ptr_type aAudio,
	screen_stack_ptr_type aScreens,
	std::shared_ptr<flappy::event_bus> aEventBus)
	: m_InputContext(aInputContext)
	, m_Screens(aScreens)
	, m_pGraphicsContext(pGraphicsContext)
	, m_pAudio(aAudio)
	, m_PlayerCountChangedObserver(std::make_shared< std::function<void(flappy::player_count_changed_event)>>(
		[this](flappy::player_count_changed_event e)
		{
			m_games.clear();

			for (int i(0); i < e.count; ++i)
				m_games.push_back(std::shared_ptr<flappy::game>(new flappy::game(m_pGraphicsContext,
					m_InputContext,
					m_pAudio)));
		}))
	, m_pBlackBGScene(gdk::graphics::context::scene_shared_ptr_type(std::move(pGraphicsContext->make_scene())))
	, m_pBlackBGCamera(std::shared_ptr<gdk::camera>(std::move(pGraphicsContext->make_camera())))
{
	(*m_PlayerCountChangedObserver)({ 1 });

	aEventBus->add_player_count_changed_observer(m_PlayerCountChangedObserver);

	m_pBlackBGCamera->set_clear_color({});

	m_pBlackBGScene->add_camera(m_pBlackBGCamera);
}

void game_screen::update(float deltaTime, float aspectRatio, std::pair<int, int> windowSize)
{
	if (m_InputContext->get_key_just_pressed(keyboard::Key::Escape)) m_Screens->pop();

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
