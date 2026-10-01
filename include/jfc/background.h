// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_FLAPPY_BACKGROUND_H
#define JFC_FLAPPY_BACKGROUND_H

#include <jfc/assets.h>

#include <gdk/graphics/context.h>
#include <gdk/graphics/entity.h>
#include <gdk/graphics/material.h>
#include <gdk/graphics/scene.h>

#include <array>

namespace flappy
{
	/// \brief controls parallax effect, clouds, etc.
	class scenery final
	{
	public:
		static constexpr size_t size = 8;

	private:
		std::array<gdk::graphics::entity_ptr_type, size> m_ParallaxEntities;
		std::array<gdk::graphics::material_ptr_type, size> m_ParallaxMaterials;

		float time = 0;

	public:
		void update(const float delta);

		scenery(gdk::graphics::context_ptr_type pContext,
			gdk::graphics::scene_ptr_type pScene,
			flappy::assets::shared_ptr aAssets);
		~scenery() = default;
	};
};

#endif
