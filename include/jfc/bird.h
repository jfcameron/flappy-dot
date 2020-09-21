// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_FLAPPY_BIRD_H
#define JFC_FLAPPY_BIRD_H

#include <gdk/graphics_context.h>
#include <gdk/scene.h>
#include <gdk/entity.h>

#include <random>

#include <array>

namespace flappy
{
	/// \brief the birdy player character
	class bird final
	{
		std::shared_ptr<gdk::entity> m_Entity;
		std::shared_ptr<gdk::material> m_Material;

		float accumulator = 0;
		int frameIndex = 0;

	public:
		void update(float delta);

		bird(gdk::graphics::context::context_shared_ptr_type pContext,
			gdk::graphics::context::scene_shared_ptr_type pScene);
	};
}

#endif
