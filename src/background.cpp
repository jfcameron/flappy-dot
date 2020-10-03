// © 2020 Joseph Cameron - All Rights Reserved
#include <jfc/background.h>

#include <gdk/material.h>

#include <jfc/Background_0.png.h>
#include <jfc/Background_1.png.h>
#include <jfc/Background_2.png.h>
#include <jfc/Background_3.png.h>
#include <jfc/Background_4.png.h>
#include <jfc/Background_5.png.h>
#include <jfc/Background_6.png.h>

#include <jfc/Floor.png.h>

#include <mutex>

using namespace flappy;
using namespace gdk;

std::array<std::shared_ptr<texture>, scenery::size> TEXTURES;

scenery::scenery(gdk::graphics::context::context_shared_ptr_type pContext,
	gdk::graphics::context::shader_program_shared_ptr_type pShader,
	gdk::graphics::context::scene_shared_ptr_type pScene)
{
	static std::once_flag flag;

	std::call_once(flag, [&]()
	{
		TEXTURES[0] = std::shared_ptr<texture>(std::move(pContext->make_texture(
			{ Background_0_png, Background_0_png + sizeof Background_0_png / sizeof Background_0_png[0] })));

		TEXTURES[1] = std::shared_ptr<texture>(std::move(pContext->make_texture(
			{ Background_1_png, Background_1_png + sizeof Background_1_png / sizeof Background_1_png[0] })));

		TEXTURES[2] = std::shared_ptr<texture>(std::move(pContext->make_texture(
			{ Background_2_png, Background_2_png + sizeof Background_2_png / sizeof Background_2_png[0] })));

		TEXTURES[3] = std::shared_ptr<texture>(std::move(pContext->make_texture(
			{ Background_3_png, Background_3_png + sizeof Background_3_png / sizeof Background_3_png[0] })));

		TEXTURES[4] = std::shared_ptr<texture>(std::move(pContext->make_texture(
			{ Background_4_png, Background_4_png + sizeof Background_4_png / sizeof Background_4_png[0] })));

		TEXTURES[5] = std::shared_ptr<texture>(std::move(pContext->make_texture(
			{ Background_5_png, Background_5_png + sizeof Background_5_png / sizeof Background_5_png[0] })));

		TEXTURES[6] = std::shared_ptr<texture>(std::move(pContext->make_texture(
			{ Background_6_png, Background_6_png + sizeof Background_6_png / sizeof Background_6_png[0] })));

		TEXTURES[7] = std::shared_ptr<texture>(std::move(pContext->make_texture(
			{ Floor_png, Floor_png + sizeof Floor_png / sizeof Floor_png[0] })));
	});

	graphics_vector2_type scale(4, 1);

	// Create materials
	for (std::remove_const<decltype(scenery::size)>::type i(0); i < size; ++i)
	{
		m_ParallaxMaterials[i] = std::shared_ptr<material>(std::move(pContext->make_material(pShader)));
		m_ParallaxMaterials[i]->setTexture("_Texture", TEXTURES[i]);
		m_ParallaxMaterials[i]->setVector2("_UVScale", scale);
	}

	auto pQuadModel = std::shared_ptr<model>(pContext->get_quad_model());

	int i(0); for (auto& a : m_ParallaxEntities)
	{
		a = std::shared_ptr<entity>(std::move(pContext->make_entity(pQuadModel, m_ParallaxMaterials[i])));

		pScene->add_entity(a);
		a->set_model_matrix(Vector3<float>{0, 0, -0.50f + (i++ * 0.01f)}, Quaternion<float>{ {0, 0, 0}}, { 4,1,1 });
	}
}

void scenery::update(const float delta)
{
	time += delta;

	int i(0); for (auto &a : m_ParallaxMaterials) 
		a->setVector2("_UVOffset", { time * (0.0333f * i++) + 0.3f, 1 });
}
