// © 2019 Joseph Cameron - All Rights Reserved

#include <cstdlib>
#include <iostream>

#include <jfc/flappy_image.png.h>
#include <jfc/background_shader.h>

#include <jfc/glfw_window.h>

#include <gdk/graphics_context.h>
#include <gdk/scene.h>

#include <GLFW/glfw3.h>

using namespace gdk;
using namespace gdk;

int main(int argc, char** argv)
{
	// Init context
	glfw_window window("flappy");

	auto pContext = graphics::context::make(graphics::context::implementation::opengl_webgl1_gles2);

	// Load embedded resources
	gdk::graphics::context::shader_program_shared_ptr_type pBackgroundShader =
		std::move(pContext->make_shader(BackgroundShaderVertexGLSL, BackgroundShaderFragmentGLSL));

	auto pTexture = std::shared_ptr<texture>(std::move(pContext->make_texture(
		{FlappyBird_png, FlappyBird_png + sizeof FlappyBird_png / sizeof FlappyBird_png[0]})));

	auto pQuadModel = std::shared_ptr<model>(pContext->get_quad_model());

	// Create materials
	auto pBackgroundMaterial = std::shared_ptr<material>(std::move(pContext->make_material(pBackgroundShader)));
	pBackgroundMaterial->setTexture("_Texture", pTexture);
	pBackgroundMaterial->setVector2("_UVScale", { 1, 1 });

	// Create entities & cameras
	auto pMainCamera = std::shared_ptr<gdk::camera>(std::move(pContext->make_camera()));

	auto pBackgroundEntity = std::shared_ptr<entity>(std::move(pContext->make_entity(pQuadModel, pBackgroundMaterial)));
	pBackgroundEntity->set_model_matrix(Vector3<float>{2., 0., -11.}, Quaternion<float>());

	// Create background scene
	auto pScene = pContext->make_scene();
	pScene->add_camera(pMainCamera);
	pScene->add_entity(pBackgroundEntity);

	float time = 0;

	while (!window.shouldClose())
	{
		glfwPollEvents();

		pBackgroundEntity->set_model_matrix(Vector3<float>{0, 0, -0.5}, Quaternion<float>{{0,0,0}});

		pMainCamera->set_projection(90, 0.01, 20, window.getAspectRatio());

		pScene->draw(window.getWindowSize());

		pBackgroundMaterial->setVector2("_UVOffset", {0, time });

		window.swapBuffer();

		time += 0.01;
	}

	return EXIT_SUCCESS;
}

