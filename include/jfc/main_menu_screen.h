// � 2020 Joseph Cameron - All Rights Reserved

#ifndef FLAPPY_MAIN_MENU_SCREEN_H
#define FLAPPY_MAIN_MENU_SCREEN_H

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

namespace gdk
{
	/// \brief entrypoint for the game's GUI
	//
	// main_menu_screen is the root of the menu system.
	class main_menu_screen final : public flappy::screen
	{
		gdk::graphics::context::scene_shared_ptr_type m_pMainScene;

		input::context::context_shared_ptr_type m_pInput;

		std::shared_ptr<gdk::camera> m_pMainCamera;

		//! header at top of screen
		std::shared_ptr<static_text_renderer> m_TitleText;
		
		//! bottom right aligned text
		std::shared_ptr<static_text_renderer> m_VersionText;
		
		//! title pane text
		std::shared_ptr<static_text_renderer> m_PromptText;

		//! title pane, start button
		std::shared_ptr<static_text_renderer> m_StartText;

		//! title pane, players count button
		std::shared_ptr<dynamic_text_renderer> m_PlayersCountText;

		//! main pane, credits button
		std::shared_ptr<static_text_renderer> m_pCreditsText;

		//! main pane, options button
		std::shared_ptr<static_text_renderer> m_pOptionsText;

		//! main pane, quit button
		std::shared_ptr<static_text_renderer> m_pQuitText;

		//! credits pane, credits text
		std::shared_ptr<static_text_renderer> m_pCreditsContextText;

		screen_stack_ptr_type m_Screens;

		screen_ptr_type m_GameScreen;
		screen_ptr_type m_OptionsScreen;

		flappy::scenery scenery;

		//! number of splitscreen players selected
		int m_PlayerCount = 1;
				
		std::shared_ptr<menu> m_menu;

		std::shared_ptr<flappy::event_bus> m_pEventBus;

	public:
		virtual void update(float delta, float aspectRatio, std::pair<int, int> windowSize) override;

		main_menu_screen(graphics::context::context_shared_ptr_type aGraphicsContext,
			input::context::context_shared_ptr_type aInputContext,
			audio::context::context_shared_ptr_type aAudioContext,
			screen_stack_ptr_type aScreens,
			screen_ptr_type aGameScreen,
			screen_ptr_type aOptionsScreen,
			std::shared_ptr<glfw_window> aGLFWWindow,
			std::shared_ptr<flappy::event_bus> aEventBus,
			flappy::assets::shared_ptr aAssets);

		virtual ~main_menu_screen() = default;
	};
}

#endif
