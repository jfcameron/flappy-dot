// © 2020 Joseph Cameron - All Rights Reserved

#ifndef FLAPPY_MAIN_MENU_SCREEN_H
#define FLAPPY_MAIN_MENU_SCREEN_H

#include <jfc/screen.h>

#include <gdk/graphics_context.h>
#include <gdk/input_context.h>
#include <gdk/audio/context.h>

#include <gdk/static_text_renderer.h>

#include <jfc/screen_stack.h>
#include <jfc/background.h>

namespace gdk
{
	class main_menu_screen final : public screen
	{
		gdk::graphics::context::scene_shared_ptr_type m_pMainScene;

		input::context::context_shared_ptr_type m_pInput;

		std::shared_ptr<gdk::camera> m_pMainCamera;


		std::shared_ptr<static_text_renderer> m_TitleText;
		std::shared_ptr<static_text_renderer> m_VersionText;
		std::shared_ptr<static_text_renderer> m_PromptText;

		screen_stack_ptr_type m_Screens;
		screen_ptr_type m_GameScreen;

		flappy::scenery scenery;

		int m_PrompCounter = 0;

	public:
		virtual void update(float delta, float aspectRatio, std::pair<int, int> windowSize) override;

		main_menu_screen(graphics::context::context_shared_ptr_type aGraphicsContext,
			input::context::context_shared_ptr_type aInputContext,
			audio::context::context_shared_ptr_type aAudioContext,
			screen_stack_ptr_type aScreens,
			screen_ptr_type aGameScreen);

		virtual ~main_menu_screen() = default;
	};
}

#endif
