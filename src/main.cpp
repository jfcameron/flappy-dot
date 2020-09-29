// © 2020 Joseph Cameron - All Rights Reserved

#include <cstdlib>
#include <iostream>
#include <functional>
#include <stack>

#include <gdk/graphics_context.h>
#include <gdk/input_context.h>
#include <gdk/audio/context.h>

#include <jfc/glfw_window.h>
#include <jfc/game_screen.h>
#include <jfc/main_menu_screen.h>
#include <jfc/icon.png.h>
#include <jfc/screen_stack.h>

#include <GLFW/glfw3.h>

#include <jfc/Coins.ogg.h>

#include <chrono>
#include <thread>

using namespace gdk;

int main(int argc, char** argv)
{
	glfw_window window("flappy::bird");

	auto pGraphicsContext = graphics::context::context_shared_ptr_type(std::move(
		graphics::context::make(graphics::context::implementation::opengl_webgl1_gles2)));

	auto pInputContext = input::context::context_shared_ptr_type(std::move(
		input::context::make(window.getPointerToGLFWWindow())));

	auto pAudioContext = audio::context::context_shared_ptr_type(std::move(
		audio::context::make(audio::context::implementation::openal)));

	auto pSound = pAudioContext->make_sound(audio::sound::encoding_type::vorbis, std::vector<unsigned char>(
		Coins_ogg, Coins_ogg + sizeof(Coins_ogg) / sizeof(Coins_ogg[0])));

	auto pEmitter = pAudioContext->make_emitter(pSound);

	pEmitter->play();

	screen_stack_ptr_type pScreens = std::make_shared<std::stack<std::shared_ptr<gdk::screen>>>(std::stack<std::shared_ptr<gdk::screen>>());

	screen_ptr_type pGameScreen = std::make_shared<gdk::game_screen>(gdk::game_screen(pGraphicsContext, 
		pInputContext, 
		pAudioContext,
		pScreens));

	screen_ptr_type pMainMenuScreen = std::make_shared<gdk::main_menu_screen>(gdk::main_menu_screen(pGraphicsContext, 
		pInputContext,
		pAudioContext,
		pScreens,
		pGameScreen));

	pScreens->push(pMainMenuScreen);
	
	using namespace std::chrono;

	float deltaTime(0);

	while (!window.shouldClose())
	{
		steady_clock::time_point t1(steady_clock::now());

		glfwPollEvents();

		pInputContext->update();

		pAudioContext->update();

		if (pScreens->size()) pScreens->top()->update(deltaTime, window.getAspectRatio(), window.getWindowSize());

		window.swapBuffer();

		//if (!pEmitter->isPlaying()) pEmitter->play();

		while (true)
		{
			steady_clock::time_point t2(steady_clock::now());
		
			duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

			if (deltaTime = time_span.count(); deltaTime > 0.01666667)
			{
				deltaTime *= 0.8; // recalibrate magic numbers so this isnt here

				break;
			}
		}
	}

	return EXIT_SUCCESS;
}
