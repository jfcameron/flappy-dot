// © 2020 Joseph Cameron - All Rights Reserved

#ifndef FLAPPY_GAME_H
#define FLAPPY_GAME_H

#include <gdk/input_context.h>
#include <gdk/intvector2.h>
#include <gdk/screen.h>
#include <gdk/graphics_context.h>
#include <gdk/input_context.h>
#include <gdk/audio/context.h>
#include <gdk/scene.h>
#include <gdk/text_map.h>
#include <gdk/static_text_renderer.h>
#include <gdk/dynamic_text_renderer.h>
#include <gdk/menu.h>

#include <jfc/game.h>
#include <jfc/Text_Sheet.png.h>
#include <jfc/Sprite_Sheet.png.h>
#include <jfc/Floor.png.h>
#include <jfc/background.h>
#include <jfc/cloud.h>
#include <jfc/bird.h>
#include <jfc/city.h>
#include <jfc/pipe.h>
#include <jfc/screen_stack.h>

#include <array>
#include <memory>
#include <functional>

namespace flappy
{
	using namespace gdk;

	class game final
	{
		enum class mode
		{
			playing,
			dead
		} m_Mode = mode::playing;

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

		/// \brief looping cloud bg effect
		std::vector<flappy::cloud> clouds;
		
		/// \brief looping city bg effect
		std::vector<flappy::city> cities;

		/// \brief score display
		std::shared_ptr<dynamic_text_renderer> pScoreText;

		/// \brief highscore display
		std::shared_ptr<dynamic_text_renderer> pHighScoreText;

		/// \brief quit
		std::shared_ptr<static_text_renderer> pQuitText;

		/// \brief retry
		std::shared_ptr<static_text_renderer> pRetryText;

		std::shared_ptr<menu> m_menu;

		// Pipe control
		size_t pipeCounter = 0;
		float pipeDelay = 0;
		std::vector<flappy::pipe> pipes;
		std::array<std::function<void(decltype(pipes)&, decltype(pipeCounter)&, decltype(pipeDelay)&, decltype(m_Random)&)>, 6> m_PipeBehaviours;
		
		bool blar();

	public:
		/// \brief called by the gameloop
		void update(float delta, 
			float aspectRatio, 
			std::pair<int, int> windowSize, 
			std::pair<float, float> vpUpperLeft, 
			std::pair<float, float> vpSize);

		game(graphics::context::context_shared_ptr_type aGraphicsContext,
			input::context::context_shared_ptr_type aInputContext,
			audio::context::context_shared_ptr_type aAudio);
	};
}

#endif
