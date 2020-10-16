// © 2020 Joseph Cameron - All Rights Reserved

#include <cstdlib>
#include <iostream>
#include <functional>
#include <stack>
#include <chrono>
#include <thread>

#include <gdk/graphics_context.h>
#include <gdk/input_context.h>
#include <gdk/audio/context.h>

#include <jfc/Coins.ogg.h>
#include <jfc/assets.h>
#include <jfc/background_music_player.h>
#include <jfc/event_bus.h>
#include <jfc/flappy_event_bus.h>
#include <jfc/game_screen.h>
#include <jfc/main_menu_screen.h>
#include <jfc/options_screen.h>
#include <jfc/glfw_window.h>
#include <jfc/icon.png.h>
#include <jfc/background.h>

#include <jfc/screen_stack.h>

#include <gdk/state_machine.h>

#include <GLFW/glfw3.h>

using namespace gdk;

int main(int argc, char** argv)
{
	// Setting up libraries
	auto window = std::shared_ptr<glfw_window>(new glfw_window("flappy dot"));

	auto pGraphicsContext = graphics::context::context_shared_ptr_type(std::move(
		graphics::context::make(graphics::context::implementation::opengl_webgl1_gles2)));

	auto pInputContext = input::context::context_shared_ptr_type(std::move(
		input::context::make(window->getPointerToGLFWWindow())));

	auto pAudioContext = audio::context::context_shared_ptr_type(std::move(
		audio::context::make(audio::context::implementation::openal)));

	auto pAssets = std::shared_ptr<flappy::assets>(new flappy::assets(pGraphicsContext, 
		pAudioContext, pInputContext));

	// Setting up top level game abstractions
	auto pEventBus = std::make_shared<flappy::event_bus>(flappy::event_bus());

	screen_ptr_type pGameScreen;
	screen_ptr_type pMainMenuScreen;
	screen_ptr_type pOptionsScreen;

	std::map<screen_ptr_type, std::string> screen_to_string;

	std::shared_ptr<gdk::screen_stack> pScreens(new gdk::screen_stack(
		[&](std::shared_ptr<gdk::screen> p)
		{
			pEventBus->propagate_screen_pushed_event({screen_to_string[p]});
		},
		[&](std::shared_ptr<gdk::screen> p)
		{
			pEventBus->propagate_screen_popped_event({ screen_to_string[p]});
		}));

	pGameScreen = decltype(pGameScreen)(new gdk::game_screen(pGraphicsContext,
		pInputContext,
		pAudioContext,
		pScreens,
		pEventBus,
		pAssets));

	pOptionsScreen = decltype(pMainMenuScreen)(new flappy::options_screen(pGraphicsContext,
		pInputContext,
		pAudioContext,
		pScreens,
		pEventBus,
		pAssets));

	pMainMenuScreen = decltype(pMainMenuScreen)(new gdk::main_menu_screen(pGraphicsContext,
		pInputContext,
		pAudioContext,
		pScreens,
		pGameScreen,
		pOptionsScreen,
		window,
		pEventBus,
		pAssets));

	screen_to_string[pGameScreen] = "GameScreen"; //TODO: remove this map, just use ptr directly. no value to strings here
	screen_to_string[pMainMenuScreen] = "MainMenu";
	screen_to_string[pOptionsScreen] = "OptionsScreen";

	flappy::background_music_player music(pEventBus, pAudioContext);

	pScreens->push(pMainMenuScreen);

	// play a start up noise
	auto pEmitter = pAudioContext->make_emitter(pAssets->get_coin_sound());
	pEmitter->play();

	for (float deltaTime(0); !window->shouldClose();)
	{
		using namespace std::chrono;

		steady_clock::time_point t1(steady_clock::now());

		glfwPollEvents();

		music.update();

		pInputContext->update();

		pAudioContext->update();

		pScreens->update(deltaTime, 
			window->getAspectRatio(), 
			window->getWindowSize());
			
		window->swapBuffer();

		while (true)
		{
			steady_clock::time_point t2(steady_clock::now());
		
			duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

			if (deltaTime = time_span.count(); deltaTime > 0.01666667) break;
		}
	}

	return EXIT_SUCCESS;
}
