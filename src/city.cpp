// © 2020 Joseph Cameron - All Rights Reserved
#include <jfc/city.h>

#include <chrono>
#include <stdexcept>

using namespace flappy;
using namespace gdk;

static const graphics::vector2_type CITY_GRAPHIC_1(0, 2);
static const graphics::vector2_type CITY_GRAPHIC_2(1, 2);

void city::randomizeGraphic()
{
	m_Position.x = 1.5f + (0.25f * static_cast<float>(m_Random() % 10));
	
	graphics::vector2_type graphic;

	switch (m_Random() % 2)
	{
		case 0: graphic = CITY_GRAPHIC_1; break;
		case 1: graphic = CITY_GRAPHIC_2; break;
		
		default: throw std::runtime_error("there are only two city graphics!");
	}

	const auto scale(0.1f + (0.025f * static_cast<float>(m_Random() % 2)));
	
	m_Material->set_vector2("_UVOffset", graphic);

	m_Speed = 0.2f;

	m_Scale = graphics::vector2_type(scale + (0.02f * m_Speed));

	m_Position.y = 0.0045f;

	m_Position.z = -0.47f;
}

city::city(gdk::graphics::context_ptr_type pContext,
	gdk::graphics::scene_ptr_type pScene,
	flappy::assets::shared_ptr aassets)
{
	m_Material = pContext->make_material(aassets->get_alpha_cutoff_shader());

	auto pTexture = aassets->get_spritesheet();

	m_Material->set_texture("_Texture", pTexture);
	m_Material->set_vector2("_UVScale", { 0.25f, 0.25f });

	m_Entity = pContext->make_entity(aassets->get_quad_model(), m_Material);

	pScene->add(m_Entity);

	m_Random.seed(static_cast<std::default_random_engine::result_type>(
		std::chrono::system_clock::now().time_since_epoch().count()));

	randomizeGraphic();
}

void city::update(const float delta)
{
	m_Entity->set_transform(m_Position, graphics::quaternion_type::identity, {m_Scale.x, m_Scale.y, 1});
	
	m_Position.x -= delta * m_Speed;

	if (m_Position.x < -2)
	{
		randomizeGraphic();
	}
}
