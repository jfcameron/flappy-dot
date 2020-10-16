// © 2020 Joseph Cameron - All Rights Reserved
#include <jfc/bird.h>

#include <jfc/Sprite_Sheet.png.h>
#include <jfc/jump.ogg.h>

using namespace gdk;
using namespace flappy;

/// \brief limit to the player's downward speed
static constexpr float player_gravity_speed_limit(-0.75f);

/// \brief limit to the player's downward acceleration
static constexpr float player_gravity_acceleration(0.155f);

/// \brief upward vertical speed assigned to the player when pressing the jump button
static constexpr float player_jump_speed(0.03f);

/// TODO Move to animator2d class
static const std::array<graphics_vector2_type, 4> FLAPPING_ANIMATION{
	graphics_vector2_type(1, 0),
	graphics_vector2_type(0, 0),
	graphics_vector2_type(2, 0),
	graphics_vector2_type(0, 0)
};
////

bird::bird(gdk::graphics::context::context_shared_ptr_type pContext,
	gdk::graphics::context::scene_shared_ptr_type pScene,
	gdk::input::context::context_shared_ptr_type pInput,
	gdk::audio::context::context_shared_ptr_type pAudio,
	flappy::assets::shared_ptr aAssets)
	: m_pInput(pInput)
	, m_state(bird::state::alive)
{
	m_Position.x = player_x;

	m_Material = std::shared_ptr<material>(std::move(pContext->make_material(pContext->get_alpha_cutoff_shader())));

	m_Material->setTexture("_Texture", aAssets->get_spritesheet());
	m_Material->setVector2("_UVScale", { 0.25, 0.245 });
	m_Material->setVector2("_UVOffset", { 0, 0 });

	m_Entity = decltype(m_Entity)(std::move(pContext->make_entity(std::shared_ptr<model>(pContext->get_quad_model()), m_Material)));

	pScene->add_entity(m_Entity);

	m_JumpSFX = pAudio->make_emitter(aAssets->get_flapsound());
}

void bird::update(float delta, std::vector<pipe> pipes)
{
	switch (m_state.get())
	{
		case state::alive:
		{
			// Animate
			{
				accumulator += delta;

				if (accumulator > 0.25f)
				{
					accumulator = 0;

					if (++frameIndex >= FLAPPING_ANIMATION.size()) frameIndex = 0;

					m_Material->setVector2("_UVOffset", FLAPPING_ANIMATION[frameIndex]);
				}
			}

			// Handle acceleration
			{
				if (m_VerticalSpeed > player_gravity_speed_limit)
					m_VerticalSpeed -= delta * player_gravity_acceleration;

				if (m_pInput->get_key_just_pressed(gdk::keyboard::Key::Space))
				{
					m_VerticalSpeed = player_jump_speed;

					m_JumpSFX->play();
				}

				m_Position.y += m_VerticalSpeed;
			}

			// Handle collision
			{
				// check too high or too low
				if (m_Position.y <= -0.5f || m_Position.y >= +0.7f) m_state.set(state::dead);
				else
				{
					// check pipe collisions
					auto wpos = get_world_position();

					for (auto& pipe : pipes)
					{
						if (pipe.check_collision(wpos)) m_state.set(state::dead);
					}
				}
			}

			// apply translation to the graphics entity
			m_Entity->set_model_matrix({ m_Position.x, m_Position.y, -0.01f },
				{ {0, 0, -m_VerticalSpeed * 40} },
				{ 0.2, 0.2, 0 });
		} break;

		case state::dead:
		{
			//TODO: some sort of death animation.
		} break;
	}
}

void bird::add_observer(decltype(m_state)::observer_ptr p)
{
	m_state.add_observer(p);
}

gdk::graphics_mat4x4_type bird::get_world_position()
{
	graphics_mat4x4_type tra;

	tra.translate({ m_Position.x, m_Position.y, 0 });
	
	graphics_mat4x4_type world = (tra);

	return tra;
}
