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

using namespace flappy;
using namespace gdk;

scenery::scenery(gdk::graphics::context::context_shared_ptr_type pContext,
	gdk::graphics::context::shader_program_shared_ptr_type pShader,
	gdk::graphics::context::scene_shared_ptr_type pScene)
{
	graphics_vector2_type scale(4, 1);

	// Create materials
	m_ParallaxMaterials[0] = std::shared_ptr<material>(std::move(pContext->make_material(pShader)));
	auto pTexture = std::shared_ptr<texture>(std::move(pContext->make_texture(
		{ Background_0_png, Background_0_png + sizeof Background_0_png / sizeof Background_0_png[0] })));
	m_ParallaxMaterials[0]->setTexture("_Texture", pTexture);
	m_ParallaxMaterials[0]->setVector2("_UVScale", scale);

	m_ParallaxMaterials[1] = std::shared_ptr<material>(std::move(pContext->make_material(pShader)));
	pTexture = pTexture = std::shared_ptr<texture>(std::move(pContext->make_texture(
		{ Background_1_png, Background_1_png + sizeof Background_1_png / sizeof Background_1_png[0] })));
	m_ParallaxMaterials[1]->setTexture("_Texture", pTexture);
	m_ParallaxMaterials[1]->setVector2("_UVScale", scale);

	m_ParallaxMaterials[2] = std::shared_ptr<material>(std::move(pContext->make_material(pShader)));
	pTexture = pTexture = std::shared_ptr<texture>(std::move(pContext->make_texture(
		{ Background_2_png, Background_2_png + sizeof Background_2_png / sizeof Background_2_png[0] })));
	m_ParallaxMaterials[2]->setTexture("_Texture", pTexture);
	m_ParallaxMaterials[2]->setVector2("_UVScale", scale);

	m_ParallaxMaterials[3] = std::shared_ptr<material>(std::move(pContext->make_material(pShader)));
	pTexture = pTexture = std::shared_ptr<texture>(std::move(pContext->make_texture(
		{ Background_3_png, Background_3_png + sizeof Background_3_png / sizeof Background_3_png[0] })));
	m_ParallaxMaterials[3]->setTexture("_Texture", pTexture);
	m_ParallaxMaterials[3]->setVector2("_UVScale", scale);

	m_ParallaxMaterials[4] = std::shared_ptr<material>(std::move(pContext->make_material(pShader)));
	pTexture = pTexture = std::shared_ptr<texture>(std::move(pContext->make_texture(
		{ Background_4_png, Background_4_png + sizeof Background_4_png / sizeof Background_4_png[0] })));
	m_ParallaxMaterials[4]->setTexture("_Texture", pTexture);
	m_ParallaxMaterials[4]->setVector2("_UVScale", scale);

	m_ParallaxMaterials[5] = std::shared_ptr<material>(std::move(pContext->make_material(pShader)));
	pTexture = pTexture = std::shared_ptr<texture>(std::move(pContext->make_texture(
		{ Background_5_png, Background_5_png + sizeof Background_5_png / sizeof Background_5_png[0] })));
	m_ParallaxMaterials[5]->setTexture("_Texture", pTexture);
	m_ParallaxMaterials[5]->setVector2("_UVScale", scale);

	m_ParallaxMaterials[6] = std::shared_ptr<material>(std::move(pContext->make_material(pShader)));
	pTexture = pTexture = std::shared_ptr<texture>(std::move(pContext->make_texture(
		{ Background_6_png, Background_6_png + sizeof Background_6_png / sizeof Background_6_png[0] })));
	m_ParallaxMaterials[6]->setTexture("_Texture", pTexture);
	m_ParallaxMaterials[6]->setVector2("_UVScale", scale);

	m_ParallaxMaterials[7] = std::shared_ptr<material>(std::move(pContext->make_material(pShader)));
	pTexture = pTexture = std::shared_ptr<texture>(std::move(pContext->make_texture(
		{ Floor_png, Floor_png + sizeof Floor_png / sizeof Floor_png[0] })));
	m_ParallaxMaterials[7]->setTexture("_Texture", pTexture);
	m_ParallaxMaterials[7]->setVector2("_UVScale", scale);

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