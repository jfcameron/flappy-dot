// © 2020 Joseph Cameron - All Rights Reserved
#include <jfc/bird.h>


using namespace gdk;
using namespace flappy;

/// \brief limit to the player's downward speed
static constexpr float player_gravity_speed_limit(-0.75f);

/// \brief limit to the player's downward acceleration
static constexpr float player_gravity_acceleration(0.155f);

/// \brief upward vertical speed assigned to the player when pressing the jump button
static constexpr float player_jump_speed(0.03f);

/// TODO Move to animator2d class
static const std::array<graphics::vector2_type, 4> FLAPPING_ANIMATION{
	graphics::vector2_type(1, 0),
	graphics::vector2_type(0, 0),
	graphics::vector2_type(2, 0),
	graphics::vector2_type(0, 0)
};
////

bird::bird(gdk::graphics::context_ptr_type pContext,
	gdk::graphics::scene_ptr_type pScene,
	gdk::input::context_ptr_type pInput,
	gdk::audio::scene_shared_ptr_type pAudio,
	flappy::assets::shared_ptr aAssets)
	: m_state(bird::state::alive)
	, m_pInput(pInput)
{
	m_Position.x = player_x;

	m_Material = pContext->make_material(aAssets->get_alpha_cutoff_shader());

	m_Material->set_texture("_Texture", aAssets->get_spritesheet());
	m_Material->set_vector2("_UVScale", { 0.25f, 0.245f });
	m_Material->set_vector2("_UVOffset", { 0, 0 });

	m_Entity = pContext->make_entity(aAssets->get_quad_model(), m_Material);

	pScene->add(m_Entity);

	m_JumpSFX = pAudio->make_emitter(aAssets->get_flapsound());
}

void bird::update(float delta, const std::vector<pipe> &pipes)
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

					if (++frameIndex >= static_cast<int>(FLAPPING_ANIMATION.size())) frameIndex = 0;

					m_Material->set_vector2("_UVOffset", FLAPPING_ANIMATION[frameIndex]);
				}
			}

			// Handle acceleration
			{
				if (m_VerticalSpeed > player_gravity_speed_limit)
					m_VerticalSpeed -= delta * player_gravity_acceleration;

				if (m_pInput->key_just_pressed(gdk::input::keyboard::key::space))
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
					for (const auto& pipe : pipes)
					{
						if (pipe.check_collision(m_Position)) m_state.set(state::dead);
					}
				}
			}

			// apply translation to the graphics entity
			m_Entity->set_transform({ m_Position.x, m_Position.y, -0.01f },
				graphics::quaternion_type::from_euler({ 0, 0, m_VerticalSpeed * 40 }),
				{ 0.2f, 0.2f, 1 });
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
