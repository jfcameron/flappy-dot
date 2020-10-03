// © 2020 Joseph Cameron - All Rights Reserved
#include <jfc/background.h>

#include <gdk/material.h>

using namespace flappy;
using namespace gdk;

scenery::scenery(gdk::graphics::context::context_shared_ptr_type pContext,
	gdk::graphics::context::shader_program_shared_ptr_type pShader,
	gdk::graphics::context::scene_shared_ptr_type pScene,
	flappy::assets::shared_ptr aAssets)
{	
	graphics_vector2_type scale(4, 1);

	// Create materials
	for (std::remove_const<decltype(scenery::size)>::type i(0); i < size; ++i)
	{
		m_ParallaxMaterials[i] = std::shared_ptr<material>(std::move(pContext->make_material(pShader)));
		m_ParallaxMaterials[i]->setTexture("_Texture", aAssets->get_bglayertextures()[i]);
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
