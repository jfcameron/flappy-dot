// © 2020 Joseph Cameron - All Rights Reserved

#ifndef FLAPPY_OPTIONS_SCREEN_H
#define FLAPPY_OPTIONS_SCREEN_H

#include <gdk/screen.h>

#include <jfc/assets.h>
#include <gdk/graphics_context.h>
#include <gdk/input_context.h>
#include <gdk/audio/context.h>
#include <gdk/menu.h>
#include <gdk/dynamic_text_renderer.h>
#include <gdk/static_text_renderer.h>

#include <jfc/screen_stack.h>
#include <jfc/background.h>
#include <jfc/glfw_window.h>
#include <jfc/flappy_screen.h>
#include <jfc/flappy_event_bus.h>

namespace flappy
{
	class options_screen final : public flappy::screen
	{
		gdk::graphics::context::scene_shared_ptr_type m_pMainScene;

		input::context::context_shared_ptr_type m_pInput;

		std::shared_ptr<gdk::camera> m_pMainCamera;

		screen_stack_ptr_type m_Screens;

		std::shared_ptr<menu> m_menu;

		std::shared_ptr<flappy::event_bus> m_pEventBus;

		//flappy::scenery scenery;

		//! header at top of screen
		//std::shared_ptr<static_text_renderer> m_TitleText;
		
	public:
		virtual void update(float delta, float aspectRatio, std::pair<int, int> windowSize) override;

		options_screen(graphics::context::context_shared_ptr_type aGraphicsContext,
			input::context::context_shared_ptr_type aInputContext,
			audio::context::context_shared_ptr_type aAudioContext,
			screen_stack_ptr_type aScreens,
			std::shared_ptr<flappy::event_bus> aEventBus,
			flappy::assets::shared_ptr aAssets);

		virtual ~options_screen() = default;
	};
}

#endif
