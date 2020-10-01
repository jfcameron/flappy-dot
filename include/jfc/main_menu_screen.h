// © 2020 Joseph Cameron - All Rights Reserved

#ifndef FLAPPY_MAIN_MENU_SCREEN_H
#define FLAPPY_MAIN_MENU_SCREEN_H

#include <gdk/screen.h>

#include <gdk/graphics_context.h>
#include <gdk/input_context.h>
#include <gdk/audio/context.h>
#include <gdk/menu.h>
#include <gdk/static_text_renderer.h>

#include <jfc/screen_stack.h>
#include <jfc/background.h>

#include <jfc/glfw_window.h>

namespace gdk
{
	class main_menu_screen final : public screen
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

		//! title pane, credits button
		std::shared_ptr<static_text_renderer> m_pCreditsText;

		//! title pane, quit button
		std::shared_ptr<static_text_renderer> m_pQuitText;

		//! Used to determine which text item shoudl blink
		std::shared_ptr<static_text_renderer> m_pCurrentText;

		screen_stack_ptr_type m_Screens;
		screen_ptr_type m_GameScreen;

		flappy::scenery scenery;

		int m_PrompCounter = 0;

		//! used to hide/show current menu element
		bool m_BlinkStatus = true;

		std::shared_ptr<menu> m_menu;

	public:
		virtual void update(float delta, float aspectRatio, std::pair<int, int> windowSize) override;



		main_menu_screen(graphics::context::context_shared_ptr_type aGraphicsContext,
			input::context::context_shared_ptr_type aInputContext,
			audio::context::context_shared_ptr_type aAudioContext,
			screen_stack_ptr_type aScreens,
			screen_ptr_type aGameScreen,
			std::shared_ptr<glfw_window> aGLFWWindow);

		virtual ~main_menu_screen() = default;
	};
}

#endif
