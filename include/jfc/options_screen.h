// © 2020 Joseph Cameron - All Rights Reserved

#ifndef FLAPPY_OPTIONS_SCREEN_H
#define FLAPPY_OPTIONS_SCREEN_H

#include <gdk/screen.h>

#include <jfc/assets.h>
#include <gdk/audio/scene.h>
#include <gdk/graphics/context.h>
#include <gdk/graphics/scene.h>
#include <gdk/graphics/screen_camera.h>
#include <gdk/input/context.h>
#include <gdk/menu.h>
#include <gdk/dynamic_text_renderer.h>
#include <gdk/static_text_renderer.h>

#include <jfc/screen_stack.h>
#include <jfc/background.h>
#include <jfc/flappy_screen.h>
#include <jfc/flappy_event_bus.h>

#include <gdk/input/controls.h>

#include <set>

namespace flappy
{
	/// \brief the options screen allows users to customize
	/// controls, enable/disable music.
	class options_screen final : public flappy::screen
	{
		gdk::graphics::scene_ptr_type m_pMainScene;

		input::context_ptr_type m_pInput;

		gdk::graphics::camera_ptr_type m_pMainCamera;

		screen_stack_ptr_type m_Screens;

		std::shared_ptr<menu> m_menu;

		std::shared_ptr<flappy::event_bus> m_pEventBus;

		//flappy::scenery scenery;

		std::vector<std::shared_ptr<static_text_renderer>> m_BindingNamesTexts;
		
		//! TEMPORARY. Remove when config is working well 
		// and I have thought of a proper place to store all the player controls.
		gdk::input::controls_ptr_type m_pControls;

		/// \brief pane to select a binding name from. root of the config sub menu
		pane::pane_shared_ptr select_binding_pane;

		/// \brief text used to show state of music on/off option
		std::shared_ptr<dynamic_text_renderer> m_MusicText;

		/// \brief text used to show state of volume % option
		std::shared_ptr<dynamic_text_renderer> m_VolumeText;

		/// \brief player config selector
		std::shared_ptr<dynamic_text_renderer> m_PlayerConfigSelect;
		
		/// \brief indicates whether or not the current player's controls react to keyboard inputs
		std::shared_ptr<dynamic_text_renderer> m_ActivateKeyboardText;

		/// \brief indicates whether or not the current player's controls react to mouse inputs 
		std::shared_ptr<dynamic_text_renderer> m_ActivateMouseText;

		/// \brief text that indicates which gamepad is selected for the current player
		std::shared_ptr<dynamic_text_renderer> m_SelectGamepadText;

		/// \brief text showing binding option
		std::shared_ptr<dynamic_text_renderer> m_ChangeBindingsText;

		/// \brief root pane of the options menu
		pane::pane_shared_ptr m_main_pane;

		/// \brief index of the player whose controls the user wants to configure
		size_t m_player_to_configure = 0;
		
	public:
		virtual void update(float delta, float aspectRatio, std::pair<int, int> windowSize) override;

		options_screen(graphics::context_ptr_type aGraphicsContext,
			input::context_ptr_type aInputContext,
			audio::scene_shared_ptr_type aAudio,
			screen_stack_ptr_type aScreens,
			std::shared_ptr<flappy::event_bus> aEventBus,
			flappy::assets::shared_ptr aAssets);

		virtual ~options_screen() = default;
	};
}

#endif
