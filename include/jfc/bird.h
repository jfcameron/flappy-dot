// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_FLAPPY_BIRD_H
#define JFC_FLAPPY_BIRD_H

#include <gdk/graphics_context.h>
#include <gdk/audio/context.h>
#include <gdk/scene.h>
#include <gdk/entity.h>
#include <gdk/input_context.h>
#include <gdk/state_machine.h>

#include <jfc/pipe.h>
#include <jfc/assets.h>

#include <random>
#include <array>

namespace flappy
{
	/// \brief the birdy player character
	class bird final
	{
	public:
		/// \brief fixed horizontal position occupied by the player
		static constexpr float player_x = -0.25f;

		/// \brief bird's possible states, managed by the state machine
		enum class state
		{
			alive,
			dead
		};

		/// \brief type used by the state machine
		using state_machine_type = jfc::state_machine<state>;

	private:
		/// \brief wrapper for state. allows observers to see when state changes
		state_machine_type m_state;

		/// \brief ptr to input system
		gdk::input::context::context_shared_ptr_type m_pInput;

		/// \brief ptr to the graphics entity
		std::shared_ptr<gdk::entity> m_Entity;

		/// \brief material used to decorate the entity
		std::shared_ptr<gdk::material> m_Material;

		/// \brief emits the jump sound effect
		std::shared_ptr<gdk::audio::emitter> m_JumpSFX;

		//TODO: move to an animator2d class
		float accumulator = 0;
		int frameIndex = 0;
		//

		/// \brief speed of the bird, affected by gravity and jumping
		float m_VerticalSpeed = 0;

		/// \brief x,y position of the bird.
		gdk::Vector2<float> m_Position;
		
		gdk::graphics_mat4x4_type get_world_position();

	public:
		void add_observer(decltype(m_state)::observer_ptr p);

		void update(float delta, std::vector<pipe> pipes);

		bird(gdk::graphics::context::context_shared_ptr_type pContext,
			gdk::graphics::context::scene_shared_ptr_type pScene,
			gdk::input::context::context_shared_ptr_type pInput,
			gdk::audio::context::context_shared_ptr_type pAudio,
			flappy::assets::shared_ptr aAssets);
	};
}

#endif
