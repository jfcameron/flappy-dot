// © 2020 Joseph Cameron - All Rights Reserved

#include <cstddef>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>

#include <gdk/audio/openal_context.h>
#include <gdk/graphics/ext/png.h>
#include <gdk/graphics/webgl1es2_context.h>
#include <gdk/input/glfw_context.h>
#include <gdk/timing/game_loop.h>
#include <gdk/windowing/impl_glfw_window.h>

#include <jfc/assets.h>
#include <jfc/background_music_player.h>
#include <jfc/event_bus.h>
#include <jfc/flappy_event_bus.h>
#include <jfc/game_screen.h>
#include <jfc/icon.png.h>
#include <jfc/main_menu_screen.h>
#include <jfc/options_screen.h>

#include <jfc/screen_stack.h>

using namespace gdk;

int main()
{
	// Setting up libraries
	auto window = windowing::impl_glfw_window::make("flappy dot");

	window->set_icons({[]()
	{
		auto [view, pData] = graphics::ext::make_from_png({ icon_png, sizeof icon_png });

		const auto *const pBytes = reinterpret_cast<const std::byte *>(pData->data());

		return windowing::window::icon_image_type{ view.width, view.height,
			{ pBytes, pBytes + view.width * view.height * windowing::window::icon_image_type::CHANNEL_COUNT } };
	}()});

	auto pGraphicsContext = graphics::webgl1es2_context::make();

	auto pGLFWInputContext = std::static_pointer_cast<input::glfw_context>(
		input::glfw_context::make(window->ptr_to_implementation()));
	input::context_ptr_type pInputContext = pGLFWInputContext;

	auto pAudioContext = audio::context_shared_ptr_type(audio::openal_context::make());

	auto pAudioScene = pAudioContext->make_scene();

	auto pAssets = std::make_shared<flappy::assets>(pGraphicsContext);

	// Setting up top level game abstractions
	auto pEventBus = std::make_shared<flappy::event_bus>();

	screen_ptr_type pGameScreen;
	screen_ptr_type pMainMenuScreen;
	screen_ptr_type pOptionsScreen;

	std::map<screen_ptr_type, std::string> screen_to_string;

	std::shared_ptr<gdk::screen_stack> pScreens(new gdk::screen_stack(
		[&](std::shared_ptr<gdk::screen> p)
		{
			pEventBus->propagate_screen_pushed_event({screen_to_string[p], p});
		},
		[&](std::shared_ptr<gdk::screen> p)
		{
			pEventBus->propagate_screen_popped_event({ screen_to_string[p]});
		}));

	pGameScreen = decltype(pGameScreen)(new gdk::game_screen(pGraphicsContext,
		pInputContext,
		pAudioScene,
		pScreens,
		pEventBus,
		pAssets));

	pOptionsScreen = decltype(pMainMenuScreen)(new flappy::options_screen(pGraphicsContext,
		pInputContext,
		pAudioScene,
		pScreens,
		pEventBus,
		pAssets));

	pMainMenuScreen = decltype(pMainMenuScreen)(new gdk::main_menu_screen(pGraphicsContext,
		pInputContext,
		pAudioScene,
		pScreens,
		pGameScreen,
		pOptionsScreen,
		window,
		pEventBus,
		pAssets));

	screen_to_string[pGameScreen] = "GameScreen"; //TODO: remove this map, just use ptr directly. no value to strings here
	screen_to_string[pMainMenuScreen] = "MainMenu";
	screen_to_string[pOptionsScreen] = "OptionsScreen";

	flappy::background_music_player music(pEventBus, pAudioScene);

	pScreens->push(pMainMenuScreen);

	// play a start up noise
	auto pEmitter = pAudioScene->make_emitter(pAssets->get_coin_sound());
	pEmitter->play();

	timing::game_loop(timing::frames_per_second{60}, [&](const timing::game_loop::frame aFrame)
	{
		const auto deltaTime = static_cast<float>(aFrame.delta);

		windowing::impl_glfw_window::poll_events();

		music.update();

		pGLFWInputContext->update();

		pAudioScene->update();

		pScreens->update(deltaTime, 
			static_cast<float>(window->aspect_ratio()), 
			window->window_size());
			
		window->swap_buffers();

		return window->should_close();
	}).run();

	return EXIT_SUCCESS;
}
