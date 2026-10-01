// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_FLAPPY_CITY_H
#define JFC_FLAPPY_CITY_H

#include <gdk/graphics/context.h>
#include <gdk/graphics/entity.h>
#include <gdk/graphics/material.h>
#include <gdk/graphics/scene.h>

#include <jfc/assets.h>

#include <random>

#include <array>

namespace flappy
{
	/// \brief models a single city
	/// randomizes appearance and speed
	class city final
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

		city(gdk::graphics::context_ptr_type pContext,
			gdk::graphics::scene_ptr_type pScene,
			flappy::assets::shared_ptr aassets);
		~city() = default;
	};
};

#endif
