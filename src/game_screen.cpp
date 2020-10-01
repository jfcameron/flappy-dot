#include <jfc/game_screen.h>

using namespace gdk;

game_screen::game_screen(graphics::context::context_shared_ptr_type pGraphicsContext,
	input::context::context_shared_ptr_type aInputContext,
	audio::context::context_shared_ptr_type aAudio,
	screen_stack_ptr_type aScreens)
	: m_InputContext(aInputContext)
	, m_Screens(aScreens)
{
	for (int i(0); i < 4; ++i)
		m_games.push_back(flappy::game(pGraphicsContext, 
			aInputContext,
			aAudio,
			aScreens));
}

void game_screen::update(float deltaTime, float aspectRatio, std::pair<int, int> windowSize)
{
	if (m_InputContext->get_key_just_pressed(keyboard::Key::Escape)) m_Screens->pop();

	struct screen_layout
	{
		std::pair<float, float> winsizeScale;
		std::pair<float, float> topLeft;
		std::pair<float, float> size;

		

		//float aspectRatio;
		
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
			{{0.5f,0.5f}, {0.5f,0.0f},{0.5f,0.5f}},
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

	std::pair<int, int> size = { 1, 1 };

	auto zeroedPlayerCount = m_games.size() - 1;

	for (decltype(m_games)::size_type i(0); i < m_games.size(); ++i)
	{
		auto ratio =		
			(layouts[zeroedPlayerCount][i].winsizeScale.first * static_cast<float>(windowSize.first))/
			(layouts[zeroedPlayerCount][i].winsizeScale.second * static_cast<float>(windowSize.second));

		m_games[i].update(deltaTime,
			ratio,
			windowSize,
			layouts[zeroedPlayerCount][i].topLeft,
			layouts[zeroedPlayerCount][i].size);
	}
}
