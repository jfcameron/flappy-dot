// © 2020 Joseph Cameron - All Rights Reserved
#include <jfc/background.h>

using namespace flappy;
using namespace gdk;

scenery::scenery(gdk::graphics::context_ptr_type pContext,
	gdk::graphics::scene_ptr_type pScene,
	flappy::assets::shared_ptr aAssets)
{	
	const graphics::vector2_type scale(4, 1);

	// Create materials
	for (std::remove_const<decltype(scenery::size)>::type i(0); i < size; ++i)
	{
		m_ParallaxMaterials[i] = pContext->make_material(aAssets->get_alpha_cutoff_shader());
		m_ParallaxMaterials[i]->set_texture("_Texture", aAssets->get_bglayertextures()[i]);
		m_ParallaxMaterials[i]->set_vector2("_UVScale", scale);
		m_ParallaxMaterials[i]->set_vector2("_UVOffset", { 0, 0 });
	}

	auto pQuadModel = aAssets->get_quad_model();

	int i(0); for (auto& a : m_ParallaxEntities)
	{
		a = pContext->make_entity(pQuadModel, m_ParallaxMaterials[i]);

		pScene->add(a);
		a->set_transform({0, 0, -0.50f + (static_cast<float>(i++) * 0.01f)}, graphics::quaternion_type::identity, { 4, 1, 1 });
	}
}

void scenery::update(const float delta)
{
	time += delta;

	int i(0); for (auto &a : m_ParallaxMaterials) 
		a->set_vector2("_UVOffset", { time * (0.0333f * static_cast<float>(i++)) + 0.3f, 1 });
}
