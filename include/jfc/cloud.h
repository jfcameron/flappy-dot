// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_FLAPPY_CLOUD_H
#define JFC_FLAPPY_CLOUD_H

#include <gdk/graphics/context.h>
#include <gdk/graphics/entity.h>
#include <gdk/graphics/material.h>
#include <gdk/graphics/scene.h>

#include <jfc/assets.h>

#include <random>
#include <array>

namespace flappy
{
	/// \brief models a single cloud
	/// randomizes appearance and speed
	class cloud final
	{
		//! instanced pseudo random number generator
		std::default_random_engine m_Random;

		//! position in the scene
		gdk::graphics::entity_ptr_type m_Entity;

		//! shader and uniform data
		gdk::graphics::material_ptr_type m_Material;

		gdk::graphics::vector2_type m_Scale = { 1, 1 };
		gdk::graphics::vector3_type m_Position;

		float m_Speed = 1;

		void randomizeGraphic();

	public:
		void update(const float delta);

		cloud(gdk::graphics::context_ptr_type pContext,
			gdk::graphics::scene_ptr_type pScene,
			flappy::assets::shared_ptr aAssets);
		~cloud() = default;
	};
};

#endif
