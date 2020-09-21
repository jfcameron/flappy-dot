// © 2019 Joseph Cameron - All Rights Reserved

#include <cstdlib>
#include <iostream>

#include <jfc/flappy_image.png.h>

#include <jfc/glfw_window.h>

#include <gdk/graphics_context.h>
#include <gdk/scene.h>

#include <GLFW/glfw3.h>

using namespace gdk;
using namespace gdk;

int main(int argc, char** argv)
{
	// Init context
	glfw_window window("basic rendering demo");

	auto pContext = graphics::context::make(graphics::context::implementation::opengl_webgl1_gles2);

	// Create background scene
	auto pScene = pContext->make_scene();

	auto pCamera = std::shared_ptr<gdk::camera>(std::move(pContext->make_camera()));

	pScene->add_camera(pCamera);

	auto pAlpha = pContext->get_alpha_cutoff_shader();

	auto pUserModel = std::shared_ptr<model>(pContext->get_quad_model());

	auto pTexture = std::shared_ptr<texture>(std::move(pContext->make_texture(
		{ FlappyBird_png, FlappyBird_png + sizeof FlappyBird_png / sizeof FlappyBird_png[0] }
	)));

	auto pMaterial = std::shared_ptr<material>(std::move(pContext->make_material(pAlpha)));
	pMaterial->setTexture("_Texture", pTexture);
	pMaterial->setVector2("_UVScale", { 1, 1 });

	auto pEntity = std::shared_ptr<entity>(std::move(pContext->make_entity(pUserModel, pMaterial)));

	pEntity->set_model_matrix(Vector3<float>{2., 0., -11.}, Quaternion<float>());

	pScene->add_entity(pEntity);

	float time = 0;

	while (!window.shouldClose())
	{
		glfwPollEvents();

		pEntity->set_model_matrix(Vector3<float>{0, 0, -0.5}, Quaternion<float>{{0,0,0}});

		pCamera->set_projection(90, 0.01, 20, window.getAspectRatio());

		pScene->draw(window.getWindowSize());

		pMaterial->setVector2("_UVOffset", {0, time });

		window.swapBuffer();

		time += 0.01;
	}

	return EXIT_SUCCESS;
}

