// © 2020 Joseph Cameron - All Rights Reserved

#ifndef FLAPPY_GAME_SCREEN_H
#define FLAPPY_GAME_SCREEN_H

#include <jfc/screen.h>

#include <gdk/graphics_context.h>
#include <gdk/input_context.h>
#include <gdk/scene.h>

#include <gdk/text_map.h>
#include <gdk/static_text_renderer.h>
#include <gdk/dynamic_text_renderer.h>

#include <jfc/Text_Sheet.png.h>
#include <jfc/Sprite_Sheet.png.h>
#include <jfc/Floor.png.h>
#include <jfc/background.h>
#include <jfc/cloud.h>
#include <jfc/bird.h>
#include <jfc/city.h>
#include <jfc/pipe.h>

#include <array>
#include <memory>
#include <functional>

namespace gdk
{
	class game_screen final : public screen
	{
		enum class mode
		{
			wait_player_ready,
			playing,
			restart_or_quit
		};

		mode m_Mode = mode::playing;

		//! instanced pseudo random number generator
		std::default_random_engine m_Random;

		//! ptr to the input abstraction
		input::context::context_shared_ptr_type pInputContext;

		//! graphics scene where gameplay takes place
		gdk::graphics::context::scene_shared_ptr_type pGameScene;

		//! graphics scene where GUI elements are rendered
		gdk::graphics::context::scene_shared_ptr_type pGUIScene;

		std::shared_ptr<gdk::camera> pMainCamera;

		flappy::scenery scenery;

		flappy::bird bird;

		std::vector<flappy::cloud> clouds;
		std::vector<flappy::city> cities;

		std::shared_ptr<dynamic_text_renderer> pText;

		// Pipe control
		size_t pipeCounter = 0;
		float pipeDelay = 0;
		std::vector<flappy::pipe> pipes;
		std::array<std::function<void(decltype(pipes) &, decltype(pipeCounter) &, decltype(pipeDelay) &, decltype(m_Random) &)>, 6> m_PipeBehaviours;

	public:
		virtual void update(float delta, float aspectRatio, std::pair<int, int> windowSize) override;

		game_screen(graphics::context::context_shared_ptr_type aGraphicsContext,
			input::context::context_shared_ptr_type aInputContext);

		virtual ~game_screen() = default;
	};
}

#endif
