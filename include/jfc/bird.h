// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_FLAPPY_BIRD_H
#define JFC_FLAPPY_BIRD_H

#include <gdk/graphics_context.h>
#include <gdk/audio/context.h>
#include <gdk/scene.h>
#include <gdk/entity.h>

#include <jfc/assets.h>

#include <random>
#include <array>

#include <gdk/input_context.h>

#include <jfc/pipe.h>

namespace flappy
{
	/// \brief the birdy player character
	class bird final
	{
		gdk::input::context::context_shared_ptr_type m_pInput;

		std::shared_ptr<gdk::entity> m_Entity;
		std::shared_ptr<gdk::material> m_Material;

		std::shared_ptr<gdk::audio::emitter> m_JumpSFX;

		float accumulator = 0;
		int frameIndex = 0;

		float m_VerticalSpeed = 0;

		gdk::Vector2<float> m_Position;

	public:
		gdk::graphics_mat4x4_type get_world_position();

		void update(float delta, std::vector<pipe> pipes);

		bird(gdk::graphics::context::context_shared_ptr_type pContext,
			gdk::graphics::context::scene_shared_ptr_type pScene,
			gdk::input::context::context_shared_ptr_type pInput,
			gdk::audio::context::context_shared_ptr_type pAudio,
			flappy::assets::shared_ptr aAssets);
	};
}

#endif
