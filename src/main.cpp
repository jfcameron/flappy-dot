// © 2019 Joseph Cameron - All Rights Reserved

#include <cstdlib>
#include <iostream>
#include <functional>

#include <jfc/Background_0.png.h>
#include <jfc/Background_1.png.h>
#include <jfc/Background_2.png.h>
#include <jfc/Background_3.png.h>
#include <jfc/Background_4.png.h>
#include <jfc/Background_5.png.h>
#include <jfc/Background_6.png.h>
#include <jfc/Floor.png.h>
#include <jfc/Sprite_Sheet.png.h>
#include <jfc/Text_Sheet.png.h>

#include <jfc/background_shader.h>

#include <jfc/glfw_window.h>

#include <gdk/graphics_context.h>
#include <gdk/scene.h>

#include <GLFW/glfw3.h>

using namespace gdk;

int main(int argc, char** argv)
{
	// Init context
	glfw_window window("flappy");

	auto pContext = graphics::context::make(graphics::context::implementation::opengl_webgl1_gles2);

	// Load embedded resources
	gdk::graphics::context::shader_program_shared_ptr_type pBackgroundShader =
		std::move(pContext->make_shader(BackgroundShaderVertexGLSL, BackgroundShaderFragmentGLSL));

	auto pQuadModel = std::shared_ptr<model>(pContext->get_quad_model());

	// Create materials
	auto pBackground0Material = std::shared_ptr<material>(std::move(pContext->make_material(pBackgroundShader)));
	auto pTexture = std::shared_ptr<texture>(std::move(pContext->make_texture(
		{ Background_0_png, Background_0_png + sizeof Background_0_png / sizeof Background_0_png[0] })));
	pBackground0Material->setTexture("_Texture", pTexture);
	pBackground0Material->setVector2("_UVScale",  { 1, 1});

	auto pBackground1Material = std::shared_ptr<material>(std::move(pContext->make_material(pBackgroundShader)));
	pTexture = pTexture = std::shared_ptr<texture>(std::move(pContext->make_texture(
		{ Background_1_png, Background_1_png + sizeof Background_1_png / sizeof Background_1_png[0] })));
	pBackground1Material->setTexture("_Texture", pTexture);
	pBackground1Material->setVector2("_UVScale", { 1, 1 });

	auto pBackground2Material = std::shared_ptr<material>(std::move(pContext->make_material(pBackgroundShader)));
	pTexture = pTexture = std::shared_ptr<texture>(std::move(pContext->make_texture(
		{ Background_2_png, Background_2_png + sizeof Background_2_png / sizeof Background_2_png[0] })));
	pBackground2Material->setTexture("_Texture", pTexture);
	pBackground2Material->setVector2("_UVScale", { 1, 1 });

	auto pBackground3Material = std::shared_ptr<material>(std::move(pContext->make_material(pBackgroundShader)));
	pTexture = pTexture = std::shared_ptr<texture>(std::move(pContext->make_texture(
		{ Background_3_png, Background_3_png + sizeof Background_3_png / sizeof Background_3_png[0] })));
	pBackground3Material->setTexture("_Texture", pTexture);
	pBackground3Material->setVector2("_UVScale", { 1, 1 });

	auto pBackground4Material = std::shared_ptr<material>(std::move(pContext->make_material(pBackgroundShader)));
	pTexture = pTexture = std::shared_ptr<texture>(std::move(pContext->make_texture(
		{ Background_4_png, Background_4_png + sizeof Background_4_png / sizeof Background_4_png[0] })));
	pBackground4Material->setTexture("_Texture", pTexture);
	pBackground4Material->setVector2("_UVScale", { 1, 1 });

	auto pBackground5Material = std::shared_ptr<material>(std::move(pContext->make_material(pBackgroundShader)));
	pTexture = pTexture = std::shared_ptr<texture>(std::move(pContext->make_texture(
		{ Background_5_png, Background_5_png + sizeof Background_5_png / sizeof Background_5_png[0] })));
	pBackground5Material->setTexture("_Texture", pTexture);
	pBackground5Material->setVector2("_UVScale", { 1, 1 });

	auto pBackground6Material = std::shared_ptr<material>(std::move(pContext->make_material(pBackgroundShader)));
	pTexture = pTexture = std::shared_ptr<texture>(std::move(pContext->make_texture(
		{ Background_6_png, Background_6_png + sizeof Background_6_png / sizeof Background_6_png[0] })));
	pBackground6Material->setTexture("_Texture", pTexture);
	pBackground6Material->setVector2("_UVScale", { 1, 1 });

	// Create entities & cameras
	auto pMainCamera = std::shared_ptr<gdk::camera>(std::move(pContext->make_camera()));

	auto pBackgroundEntity0 = std::shared_ptr<entity>(std::move(pContext->make_entity(pQuadModel, pBackground0Material)));
	auto pBackgroundEntity1 = std::shared_ptr<entity>(std::move(pContext->make_entity(pQuadModel, pBackground1Material)));
	auto pBackgroundEntity2 = std::shared_ptr<entity>(std::move(pContext->make_entity(pQuadModel, pBackground2Material)));
	auto pBackgroundEntity3 = std::shared_ptr<entity>(std::move(pContext->make_entity(pQuadModel, pBackground3Material)));
	auto pBackgroundEntity4 = std::shared_ptr<entity>(std::move(pContext->make_entity(pQuadModel, pBackground4Material)));
	auto pBackgroundEntity5 = std::shared_ptr<entity>(std::move(pContext->make_entity(pQuadModel, pBackground5Material)));
	auto pBackgroundEntity6 = std::shared_ptr<entity>(std::move(pContext->make_entity(pQuadModel, pBackground6Material)));

	// Create background scene
	auto pScene = pContext->make_scene();
	pScene->add_camera(pMainCamera);
	pScene->add_entity(pBackgroundEntity0);
	pScene->add_entity(pBackgroundEntity1);
	pScene->add_entity(pBackgroundEntity2);
	pScene->add_entity(pBackgroundEntity3);
	pScene->add_entity(pBackgroundEntity4);
	pScene->add_entity(pBackgroundEntity5);
	pScene->add_entity(pBackgroundEntity6);

	float time = 0;

	pBackgroundEntity0->set_model_matrix(Vector3<float>{0, 0, -0.50},  Quaternion<float>{ {0, 0, 0}});
	pBackgroundEntity1->set_model_matrix(Vector3<float>{0, 0, -0.499}, Quaternion<float>{ {0, 0, 0}});
	pBackgroundEntity2->set_model_matrix(Vector3<float>{0, 0, -0.498}, Quaternion<float>{ {0, 0, 0}});
	pBackgroundEntity3->set_model_matrix(Vector3<float>{0, 0, -0.497}, Quaternion<float>{ {0, 0, 0}});
	pBackgroundEntity4->set_model_matrix(Vector3<float>{0, 0, -0.496}, Quaternion<float>{ {0, 0, 0}});
	pBackgroundEntity5->set_model_matrix(Vector3<float>{0, 0, -0.495}, Quaternion<float>{ {0, 0, 0}});
	pBackgroundEntity6->set_model_matrix(Vector3<float>{0, 0, -0.494}, Quaternion<float>{ {0, 0, 0}});

	while (!window.shouldClose())
	{
		glfwPollEvents();

		pMainCamera->set_orthographic_projection(2, 2, 0.01, 10, window.getAspectRatio());

		pScene->draw(window.getWindowSize());

		pBackground0Material->setVector2("_UVOffset", { time * 0.4f, 1 });
		pBackground1Material->setVector2("_UVOffset", { time * 0.5f, 1 });
		pBackground2Material->setVector2("_UVOffset", { time * 0.6f, 1 });
		pBackground3Material->setVector2("_UVOffset", { time * 0.7f, 1 });
		pBackground4Material->setVector2("_UVOffset", { time * 0.8f, 1 });
		pBackground5Material->setVector2("_UVOffset", { time * 0.9f, 1 });
		pBackground6Material->setVector2("_UVOffset", { time * 1.0f, 1 });

		window.swapBuffer();

		time += 0.01;
	}

	return EXIT_SUCCESS;
}
