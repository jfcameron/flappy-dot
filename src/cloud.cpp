// © 2020 Joseph Cameron - All Rights Reserved
#include <jfc/cloud.h>

#include <chrono>
#include <stdexcept>

using namespace flappy;
using namespace gdk;

static const graphics::vector2_type CLOUD_GRAPHIC_1(3, 1);
static const graphics::vector2_type CLOUD_GRAPHIC_2(2, 1);

void cloud::randomizeGraphic()
{
	m_Position.x = 2 + (0.25f * static_cast<float>(m_Random() % 8));
	
	graphics::vector2_type graphic;

	switch (m_Random() % 2)
	{
		case 0: graphic = CLOUD_GRAPHIC_1; break;
		case 1: graphic = CLOUD_GRAPHIC_2; break;
		
		default: throw std::runtime_error("there are only two cloud graphics!");
	}

	const auto scale(0.25f + (0.05f * static_cast<float>(m_Random() % 3)));
	
	m_Scale = graphics::vector2_type(scale);

	m_Position.y = 0.2f + (0.1f * static_cast<float>(m_Random() % 3));
	
	m_Material->set_vector2("_UVOffset", graphic);

	m_Speed = 0.5f + (0.3f * static_cast<float>(m_Random() % 4));

	m_Position.z = -0.490f + (m_Speed* 0.001f);
}

cloud::cloud(gdk::graphics::context_ptr_type pContext,
	gdk::graphics::scene_ptr_type pScene,
	flappy::assets::shared_ptr aAssets)
{
	m_Material = pContext->make_material(aAssets->get_alpha_cutoff_shader());

	auto pTexture = aAssets->get_spritesheet();

	m_Material->set_texture("_Texture", pTexture);
	m_Material->set_vector2("_UVScale", { 0.25f, 0.25f });

	m_Entity = pContext->make_entity(aAssets->get_quad_model(), m_Material);

	pScene->add(m_Entity);

	m_Random.seed(static_cast<std::default_random_engine::result_type>(
		std::chrono::system_clock::now().time_since_epoch().count()));

	randomizeGraphic();
}

void cloud::update(const float delta)
{
	m_Entity->set_transform(m_Position, graphics::quaternion_type::identity, {m_Scale.x, m_Scale.y, 1});
	
	m_Position.x -= delta * m_Speed;

	if (m_Position.x < -2)
	{
		randomizeGraphic();
	}
}
