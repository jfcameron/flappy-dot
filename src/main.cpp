// © 2020 Joseph Cameron - All Rights Reserved

#include <cstdlib>
#include <iostream>
#include <functional>
#include <stack>

#include <gdk/graphics_context.h>
#include <gdk/input_context.h>

#include <jfc/glfw_window.h>
#include <jfc/game_screen.h>
#include <jfc/main_menu_screen.h>
#include <jfc/icon.png.h>
#include <jfc/screen_stack.h>

#include <GLFW/glfw3.h>

using namespace gdk;

int main(int argc, char** argv)
{
	glfw_window window("flappy::bird");

	auto pGraphicsContext = graphics::context::context_shared_ptr_type(std::move(
		graphics::context::make(graphics::context::implementation::opengl_webgl1_gles2)));

	auto pInputContext = input::context::context_shared_ptr_type(std::move(
		input::context::make(window.getPointerToGLFWWindow())));

	float deltaTime = 0.01;
	
	screen_stack_ptr_type pScreens = std::make_shared<std::stack<std::shared_ptr<gdk::screen>>>(std::stack<std::shared_ptr<gdk::screen>>());

	screen_ptr_type pGameScreen = std::make_shared<gdk::game_screen>(gdk::game_screen(pGraphicsContext, pInputContext, pScreens));
	screen_ptr_type pMainMenuScreen = std::make_shared<gdk::main_menu_screen>(gdk::main_menu_screen(pGraphicsContext, 
		pInputContext,
		pScreens,
		pGameScreen));

	pScreens->push(pMainMenuScreen);
	
	while (!window.shouldClose())
	{
		glfwPollEvents();

		pInputContext->update();

		if (pScreens->size()) pScreens->top()->update(deltaTime, window.getAspectRatio(), window.getWindowSize());

		window.swapBuffer();
	}

	return EXIT_SUCCESS;
}
