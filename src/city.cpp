// © 2020 Joseph Cameron - All Rights Reserved
#include <jfc/city.h>

#include <jfc/Sprite_Sheet.png.h>

#include <chrono>

using namespace flappy;
using namespace gdk;

static const graphics_vector2_type CITY_GRAPHIC_1(0, 2);
static const graphics_vector2_type CITY_GRAPHIC_2(1, 2);

static float blar(0);

void city::randomizeGraphic()
{
	m_Position.x = 1.5f + (0.25f * (m_Random() % 10));
	
	graphics_vector2_type graphic;

	switch (m_Random() % 2)
	{
		case 0: graphic = CITY_GRAPHIC_1; break;
		case 1: graphic = CITY_GRAPHIC_2; break;
		
		default: throw std::runtime_error("there are only two city graphics!");
	}

	const auto scale(0.1f + (0.025f * (m_Random() % 2)));
	
	m_Material->setVector2("_UVOffset", graphic);

	m_Speed = 0.2f;//+(0.2f * (m_Random() % 4));

	m_Scale = { scale + (0.02f * m_Speed) };

	m_Position.y = 0.0045f;// +(m_Random() % 4 / 8);

	m_Position.z = -0.47f;// +(-m_Position.y * 0.06);
}

city::city(gdk::graphics::context::context_shared_ptr_type pContext,
	gdk::graphics::context::scene_shared_ptr_type pScene)
{
	m_Material = std::shared_ptr<material>(std::move(pContext->make_material(pContext->get_alpha_cutoff_shader())));
	auto pTexture = std::shared_ptr<texture>(std::move(pContext->make_texture(
		{ Sprite_Sheet_png, Sprite_Sheet_png + sizeof Sprite_Sheet_png / sizeof Sprite_Sheet_png[0] })));

	m_Material->setTexture("_Texture", pTexture);
	m_Material->setVector2("_UVScale", { 0.25, 0.25 });

	m_Entity = decltype(m_Entity)(std::move(pContext->make_entity(std::shared_ptr<model>(pContext->get_quad_model()), m_Material)));

	pScene->add_entity(m_Entity);

	m_Random.seed(std::chrono::system_clock::now().time_since_epoch().count());

	randomizeGraphic();
}

static float total_time(0);

void city::update(const float delta)
{
	total_time += delta * m_Speed;

	m_Entity->set_model_matrix(m_Position, {}, {m_Scale.x, m_Scale.y, 1});
	
	m_Position.x -= delta * m_Speed;

	if (m_Position.x < -2)
	{
		randomizeGraphic();
	}
}
