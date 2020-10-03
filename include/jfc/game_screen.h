// � 2020 Joseph Cameron - All Rights Reserved

#ifndef FLAPPY_GAME_SCREEN_H
#define FLAPPY_GAME_SCREEN_H

#include <gdk/screen.h>

#include <gdk/graphics_context.h>
#include <gdk/input_context.h>
#include <gdk/audio/context.h>

#include <gdk/scene.h>

#include <gdk/text_map.h>
#include <gdk/static_text_renderer.h>
#include <gdk/dynamic_text_renderer.h>

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
#include <jfc/flappy_event_bus.h>
#include <jfc/assets.h>

#include <array>
#include <memory>
#include <functional>

namespace gdk
{
	// TOOD: instead of popped/pushed, swithc to gained_top, lost_top.
	class game_screen final : public screen
	{
		/// \brief collection of games,
		std::vector<std::shared_ptr<flappy::game>> m_games;

		input::context::context_shared_ptr_type m_InputContext;
		
		screen_stack_ptr_type m_Screens;

		graphics::context::context_shared_ptr_type m_pGraphicsContext;

		audio::context::context_shared_ptr_type m_pAudio;

		/// \brief callback when the # of splitscreen players change
		std::shared_ptr <std::function<void(flappy::player_count_changed_event)>> m_PlayerCountChangedObserver;

		/// \brief used to render a black screen behind the game instances
		gdk::graphics::context::scene_shared_ptr_type m_pBlackBGScene;

		/// \brief used to render a black screen behind the game instances
		std::shared_ptr<gdk::camera> m_pBlackBGCamera;
	public:
		virtual void update(float delta, float aspectRatio, std::pair<int, int> windowSize) override;

		game_screen(graphics::context::context_shared_ptr_type aGraphicsContext,
			input::context::context_shared_ptr_type aInputContext,
			audio::context::context_shared_ptr_type aAudio,
			screen_stack_ptr_type aScreens,
			std::shared_ptr<flappy::event_bus> aEventBus,
			flappy::assets::shared_ptr aAssets);

		virtual ~game_screen() = default;
	};
}

#endif
