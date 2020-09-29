// © 2020 Joseph Cameron - All Rights Reserved
#include <jfc/bird.h>

#include <jfc/Sprite_Sheet.png.h>
#include <jfc/jump.ogg.h>

using namespace gdk;
using namespace flappy;

static const std::array<graphics_vector2_type, 4> FLAPPING_ANIMATION{
	graphics_vector2_type(1, 0),
	graphics_vector2_type(0, 0),
	graphics_vector2_type(2, 0),
	graphics_vector2_type(0, 0)
};

static std::shared_ptr<gdk::entity> coolEntity;

bird::bird(gdk::graphics::context::context_shared_ptr_type pContext,
	gdk::graphics::context::scene_shared_ptr_type pScene,
	gdk::input::context::context_shared_ptr_type pInput,
	gdk::audio::context::context_shared_ptr_type pAudio)
	: m_pInput(pInput)
{
	m_Position.x = -0.25f;

	m_Material = std::shared_ptr<material>(std::move(pContext->make_material(pContext->get_alpha_cutoff_shader())));

	auto pTexture = std::shared_ptr<texture>(std::move(pContext->make_texture(
		{ Sprite_Sheet_png, Sprite_Sheet_png + sizeof Sprite_Sheet_png / sizeof Sprite_Sheet_png[0] })));

	m_Material->setTexture("_Texture", pTexture);
	m_Material->setVector2("_UVScale", { 0.25, 0.245 });
	m_Material->setVector2("_UVOffset", { 0, 0 });

	m_Entity = decltype(m_Entity)(std::move(pContext->make_entity(std::shared_ptr<model>(pContext->get_quad_model()), m_Material)));
	pScene->add_entity(m_Entity);

	auto pSound = pAudio->make_sound(audio::sound::encoding_type::vorbis, std::vector<unsigned char>(
		jump_ogg, jump_ogg + sizeof(jump_ogg) / sizeof(jump_ogg[0])));

	m_JumpSFX = pAudio->make_emitter(pSound);
}

void bird::update(float delta, std::vector<pipe> pipes)
{
	// Animate
	accumulator += delta;

	if (accumulator > 0.35)
	{
		accumulator = 0;
	
		if (++frameIndex >= FLAPPING_ANIMATION.size()) frameIndex = 0;

		m_Material->setVector2("_UVOffset", FLAPPING_ANIMATION[frameIndex]);
	}

	// Handle acceleration
	if (m_VerticalSpeed > -1.f) m_VerticalSpeed -= delta * 0.1;

	if (m_pInput->get_key_just_pressed(gdk::keyboard::Key::Space))
	{
		m_VerticalSpeed = 0.0225f;

		m_JumpSFX->play();
	}

	m_Position.y += m_VerticalSpeed;

	m_Entity->set_model_matrix({ m_Position.x, m_Position.y, 0 }, { {0, 0, -m_VerticalSpeed * 60} }, { 0.2, 0.2, 1 });

	graphics_mat4x4_type tra;
	tra.translate({ m_Position.x, m_Position.y, 0 });
	graphics_mat4x4_type world = (tra);
	
	// Pipe collisions
	for (const auto p : pipes)
	{
		p.check_collision(world);
			//? std::cout << "collision\n"
			//: std::cout << "\n";
	}
}
