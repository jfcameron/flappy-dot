// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_FLAPPY_PIPE_H
#define JFC_FLAPPY_PIPE_H

#include <gdk/graphics/context.h>
#include <gdk/graphics/entity.h>
#include <gdk/graphics/material.h>
#include <gdk/graphics/model.h>
#include <gdk/graphics/model_data.h>
#include <gdk/graphics/scene.h>

#include <jfc/assets.h>

#include <random>
#include <array>

namespace flappy
{
	/// \brief the game's obstacles
	class pipe final
	{
		//! position in the scene
		gdk::graphics::entity_ptr_type m_Entity;

		//! shader and uniform data
		gdk::graphics::material_ptr_type m_Material;

		gdk::graphics::vector2_type m_Position;
		gdk::graphics::vector2_type m_Scale;
		float m_Rotation = 0;

	public:
		decltype(m_Position) getPosition() const;
		
		decltype(m_Scale) getScale() const;

		decltype(m_Rotation) getRotation() const;

		void update(const float delta);

		bool check_collision(const gdk::graphics::vector2_type &aWorldPosition) const;

		void set_up(const decltype(m_Position)& aPosition, const decltype(m_Rotation) aRotation);

		pipe(gdk::graphics::context_ptr_type pContext,
			gdk::graphics::scene_ptr_type pScene,
			flappy::assets::shared_ptr aAssets);

		~pipe() = default;
	};
};

#endif
