// � 2019 Joseph Cameron - All Rights Reserved

#include <cstdlib>
#include <iostream>

#include <jfc/glfw_window.h>

#include <gdk/graphics_context.h>
#include <gdk/scene.h>

#include <GLFW/glfw3.h>

using namespace gdk;
using namespace gdk;

int main(int argc, char** argv)
{
	// Separate lib, used to init GL and get a window ready for rendering on Linux/Mac/Windows
	glfw_window window("basic rendering demo");

	auto pContext = graphics::context::make(graphics::context::implementation::opengl_webgl1_gles2);

	auto pScene = pContext->make_scene();

	auto pCamera = std::shared_ptr<gdk::camera>(std::move(pContext->make_camera()));

	pScene->add_camera(pCamera);

	auto pAlpha = pContext->get_alpha_cutoff_shader();

	using vertex_attribute_type = float;
	using vertex_attribute_array_type = std::vector<vertex_attribute_type>;

	vertex_attribute_type size = 1;
	decltype(size) hsize = size / 2.;

	vertex_attribute_array_type posData({ //Quad data: vertex positon  
		size - hsize, size - hsize, 0.0f,
		0.0f - hsize, size - hsize, 0.0f,
		0.0f - hsize, 0.0f - hsize, 0.0f,
		size - hsize, size - hsize, 0.0f,
		0.0f - hsize, 0.0f - hsize, 0.0f,
		size - hsize, 0.0f - hsize, 0.0f });

	vertex_attribute_array_type uvData({ //Quad data: uvs
		1, 0,
		0, 0,
		0, 1,
		1, 0,
		0, 1,
		1, 1 });

	auto pUserModel = std::shared_ptr<model>(std::move(
		pContext->make_model({ vertex_data_view::UsageHint::Static,
		{
			{
				"a_Position",
				{
					&posData.front(),
					posData.size(),
					3
				}
			},
			{
				"a_UV",
				{
					&uvData.front(),
					uvData.size(),
					2
				}
			}
		} })));

	texture::image_data_2d_view view;
	view.width = 2;
	view.height = 2;
	view.format = texture::data_format::rgba;

	std::vector<std::underlying_type<std::byte>::type> imageData({ // raw rgba data
		0x00, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff,
		0x00, 0x00, 0x00, 0xff });

	view.data = reinterpret_cast<std::byte*>(&imageData.front());

	auto pTexture = std::shared_ptr<texture>(std::move(pContext->make_texture(view)));

	auto pMaterial = std::shared_ptr<material>(std::move(pContext->make_material(pAlpha)));

	pMaterial->setTexture("_Texture", pTexture);

	view.width = 2;
	view.height = 2;
	view.format = texture::data_format::rgba;

	imageData = decltype(imageData)({
		0x55, 0xff, 0xff, 0xff,
		0xff, 0x00, 0xff, 0xff,
		0xff, 0xff, 0x00, 0xff,
		0x00, 0x00, 0x44, 0xff });

	view.data = reinterpret_cast<std::byte*>(&imageData.front());

	auto pTexture2 = std::shared_ptr<gdk::texture>(std::move(pContext->make_texture(view)));

	auto pMaterial2 = std::shared_ptr<material>(std::move(pContext->make_material(pAlpha)));

	pMaterial2->setTexture("_Texture", pTexture2);

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

		window.swapBuffer();

		time += 0.01;
	}

	return EXIT_SUCCESS;
}

