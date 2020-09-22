// © 2020 Joseph Cameron - All Rights Reserved
#include <jfc/bird.h>

#include <jfc/Sprite_Sheet.png.h>

using namespace gdk;
using namespace flappy;

static const std::array<graphics_vector2_type, 4> FLAPPING_ANIMATION{
	graphics_vector2_type(1, 0),
	graphics_vector2_type(0, 0),
	graphics_vector2_type(2, 0),
	graphics_vector2_type(0, 0)
};

bird::bird(gdk::graphics::context::context_shared_ptr_type pContext,
	gdk::graphics::context::scene_shared_ptr_type pScene,
	gdk::input::context::context_shared_ptr_type pInput)
	: m_pInput(pInput)
{
	m_Material = std::shared_ptr<material>(std::move(pContext->make_material(pContext->get_alpha_cutoff_shader())));
	auto pTexture = std::shared_ptr<texture>(std::move(pContext->make_texture(
		{ Sprite_Sheet_png, Sprite_Sheet_png + sizeof Sprite_Sheet_png / sizeof Sprite_Sheet_png[0] })));

	m_Material->setTexture("_Texture", pTexture);
	m_Material->setVector2("_UVScale", { 0.25, 0.245 });

	m_Entity = decltype(m_Entity)(std::move(pContext->make_entity(std::shared_ptr<model>(pContext->get_quad_model()), m_Material)));

	pScene->add_entity(m_Entity);

	m_Material->setVector2("_UVOffset", { 0, 0 });
}

void bird::update(float delta)
{
	accumulator += delta;

	if (accumulator > 0.35)
	{
		accumulator = 0;
	
		if (++frameIndex >= FLAPPING_ANIMATION.size()) frameIndex = 0;

		m_Material->setVector2("_UVOffset", FLAPPING_ANIMATION[frameIndex]);
	}

	m_Entity->set_model_matrix({ 0, 0, 0 }, {{0, 0, 0}}, { 0.2, 0.2, 1 });

	if (m_pInput->get_key_down(gdk::keyboard::Key::A)) std::cout << "its down!\n";

	if (m_pInput->get_key_just_pressed(gdk::keyboard::Key::A)) std::cout << "its just down!\n";
}
