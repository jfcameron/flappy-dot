// © 2019 Joseph Cameron - All Rights Reserved

#include <cstdlib>
#include <iostream>
#include <functional>

#include <gdk/graphics_context.h>
#include <gdk/scene.h>
#include <gdk/input_context.h>

#include <jfc/Sprite_Sheet.png.h>
#include <jfc/Floor.png.h>
#include <jfc/Sprite_Sheet.png.h>
#include <jfc/Text_Sheet.png.h>
#include <jfc/background_shader.h>
#include <jfc/glfw_window.h>
#include <jfc/background.h>
#include <jfc/cloud.h>
#include <jfc/bird.h>
#include <jfc/city.h>
#include <jfc/pipe.h>

#include <GLFW/glfw3.h>

using namespace gdk;

int main(int argc, char** argv)
{
	// Init context
	glfw_window window("flappy::bird");

	auto pGraphicsContext = graphics::context::context_shared_ptr_type(std::move(
		graphics::context::make(graphics::context::implementation::opengl_webgl1_gles2)));

	auto pInputContext = input::context::context_shared_ptr_type(std::move(
		input::context::make(window.getPointerToGLFWWindow())));

	// Load embedded resources
	gdk::graphics::context::shader_program_shared_ptr_type pBackgroundShader =
		std::move(pGraphicsContext->make_shader(BackgroundShaderVertexGLSL, BackgroundShaderFragmentGLSL));

	auto pQuadModel = std::shared_ptr<model>(pGraphicsContext->get_quad_model());

	// Create entities & cameras
	auto pMainCamera = std::shared_ptr<gdk::camera>(std::move(pGraphicsContext->make_camera()));

	// Create primary scene
	auto pScene = gdk::graphics::context::scene_shared_ptr_type(std::move(pGraphicsContext->make_scene()));
	pScene->add_camera(pMainCamera);

	// Create GUI scene
	//.........

	float deltaTime = 0.01;

	flappy::scenery scenery(pGraphicsContext, pBackgroundShader, pScene);

	std::vector<flappy::cloud> clouds;

	flappy::bird bird(pGraphicsContext, pScene, pInputContext);

	flappy::pipe pipe(pGraphicsContext, pScene);
	
	for (int i(0); i < 10; ++i) clouds.push_back(flappy::cloud(pGraphicsContext, pScene));
	
	while (!window.shouldClose())
	{
		glfwPollEvents();

		pInputContext->update();

		pMainCamera->set_orthographic_projection(2, 2, 0.01, 10, window.getAspectRatio());

		pScene->draw(window.getWindowSize());

		scenery.update(deltaTime);

		bird.update(deltaTime);

		pipe.update(deltaTime);

		for (auto &cloud : clouds) cloud.update(deltaTime);
		
		window.swapBuffer();
	}

	return EXIT_SUCCESS;
}
