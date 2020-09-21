// © 2020 Joseph Cameron - All Rights Reserved
#include <jfc/cloud.h>

#include <jfc/Sprite_Sheet.png.h>

#include <chrono>

using namespace flappy;
using namespace gdk;

static const graphics_vector2_type CLOUD_GRAPHIC_1(3, 1);
static const graphics_vector2_type CLOUD_GRAPHIC_2(2, 1);

static float blar(0);

void cloud::randomizeGraphic()
{
	m_Position.x = 2 + (0.25f * (m_Random() % 8));
	
	graphics_vector2_type graphic;

	switch (m_Random() % 2)
	{
		case 0: graphic = CLOUD_GRAPHIC_1; break;
		case 1: graphic = CLOUD_GRAPHIC_2; break;
		
		default: throw std::runtime_error("there are only two cloud graphics!");
	}

	const auto scale(0.25f + (0.05f * (m_Random() % 3)));
	
	m_Scale = {scale};

	m_Position.y = 0.2f + (0.1f * (m_Random() % 3));
	
	m_Material->setVector2("_UVOffset", graphic);

	m_Speed = 0.5f + (0.3f * (m_Random() % 4));
}

cloud::cloud(gdk::graphics::context::context_shared_ptr_type pContext,
	gdk::graphics::context::scene_shared_ptr_type pScene)
{
	m_Material = std::shared_ptr<material>(std::move(pContext->make_material(pContext->get_alpha_cutoff_shader())));
	auto pTexture = std::shared_ptr<texture>(std::move(pContext->make_texture(
		{ Sprite_Sheet_png, Sprite_Sheet_png + sizeof Sprite_Sheet_png / sizeof Sprite_Sheet_png[0] })));

	m_Material->setTexture("_Texture", pTexture);
	m_Material->setVector2("_UVScale", { 0.25, 0.25 });

	m_Entity = decltype(m_Entity)(std::move(pContext->make_entity(std::shared_ptr<model>(pContext->get_quad_model()), m_Material)));

	pScene->add_entity(m_Entity);

	unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
	m_Random.seed(seed1);

	randomizeGraphic();
}

float total_time(0);

void cloud::update(const float delta)
{
	total_time += delta * m_Speed;

	m_Entity->set_model_matrix(m_Position, {}, {m_Scale.x, m_Scale.y, 1});
	
	m_Position.x -= delta * m_Speed;

	if (m_Position.x < -2)
	{
		randomizeGraphic();
	}
}
