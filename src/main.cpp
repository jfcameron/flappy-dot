// © 2019 Joseph Cameron - All Rights Reserved

#include <cstdlib>
#include <iostream>
#include <functional>

#include <jfc/Sprite_Sheet.png.h>

#include <jfc/Floor.png.h>
#include <jfc/Sprite_Sheet.png.h>
#include <jfc/Text_Sheet.png.h>

#include <jfc/background_shader.h>

#include <jfc/glfw_window.h>

#include <gdk/graphics_context.h>
#include <gdk/scene.h>

#include <jfc/background.h>
#include <jfc/cloud.h>

#include <GLFW/glfw3.h>

using namespace gdk;

int main(int argc, char** argv)
{
	// Init context
	glfw_window window("flappy");

	auto pContext = graphics::context::context_shared_ptr_type(std::move(
		graphics::context::make(graphics::context::implementation::opengl_webgl1_gles2)));

	// Load embedded resources
	gdk::graphics::context::shader_program_shared_ptr_type pBackgroundShader =
		std::move(pContext->make_shader(BackgroundShaderVertexGLSL, BackgroundShaderFragmentGLSL));

	auto pQuadModel = std::shared_ptr<model>(pContext->get_quad_model());

	// Create materials

	// Create entities & cameras
	auto pMainCamera = std::shared_ptr<gdk::camera>(std::move(pContext->make_camera()));

	// Create background scene
	auto pScene = gdk::graphics::context::scene_shared_ptr_type(std::move(pContext->make_scene()));
	pScene->add_camera(pMainCamera);

	float deltaTime = 0.01;

	flappy::scenery scenery(pContext, pBackgroundShader, pScene);

	std::vector<flappy::cloud> clouds;
	
	for (int i(0); i < 10; ++i) clouds.push_back(flappy::cloud(pContext, pScene));
	
	auto blar = flappy::cloud(pContext, pScene);

	std::cout << clouds.size();
	
	while (!window.shouldClose())
	{
		glfwPollEvents();

		pMainCamera->set_orthographic_projection(2, 2, 0.01, 10, window.getAspectRatio());

		pScene->draw(window.getWindowSize());

		scenery.update(deltaTime);

		for (auto &cloud : clouds) cloud.update(deltaTime);

		blar.update(deltaTime);

		window.swapBuffer();
	}

	return EXIT_SUCCESS;
}
