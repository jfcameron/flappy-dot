// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_FLAPPY_PIPE_H
#define JFC_FLAPPY_PIPE_H

#include <gdk/graphics_context.h>
#include <gdk/scene.h>
#include <gdk/entity.h>
#include <gdk/model.h>

#include <random>

#include <array>

namespace flappy
{
	/// \brief the game's obstacles
	class pipe final
	{
		//! instanced pseudo random number generator
		std::default_random_engine m_Random;

		//! position in the scene
		std::shared_ptr<gdk::entity> m_Entity;

		//! shader and uniform data
		std::shared_ptr<gdk::material> m_Material;

		//! model for the upwards pipe
		std::shared_ptr<gdk::model> m_UpPipeModel;

		//! model for the downwards pipe
		std::shared_ptr<gdk::model> m_DownPipeModel;

		gdk::Vector3<float> m_Position;

	public:
		void update(const float delta);

		pipe(gdk::graphics::context::context_shared_ptr_type pContext,
			gdk::graphics::context::scene_shared_ptr_type pScene);
		~pipe() = default;
	};
};

#endif