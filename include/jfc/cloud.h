// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_FLAPPY_CLOUD_H
#define JFC_FLAPPY_CLOUD_H

#include <gdk/graphics_context.h>
#include <gdk/scene.h>
#include <gdk/entity.h>

#include <random>

#include <array>

namespace flappy
{
	/// \brief models a single cloud
	class cloud final
	{
		std::default_random_engine m_Random;

		std::shared_ptr<gdk::entity> m_Entity;
		std::shared_ptr<gdk::material> m_Material;

		gdk::Vector2<float> m_Scale = gdk::Vector2<float>::One;
		gdk::Vector3<float> m_Position;

		float m_Speed = 1;

		void randomizeGraphic();

	public:
		void update(const float delta);

		cloud(gdk::graphics::context::context_shared_ptr_type pContext,
			gdk::graphics::context::scene_shared_ptr_type pScene);
		~cloud() = default;
	};
};

#endif