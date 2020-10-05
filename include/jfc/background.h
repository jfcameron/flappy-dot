// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_FLAPPY_BACKGROUND_H
#define JFC_FLAPPY_BACKGROUND_H

#include <jfc/assets.h>

#include <gdk/graphics_context.h>
#include <gdk/scene.h>
#include <gdk/entity.h>

#include <array>

namespace flappy
{
	/// \brief controls parallax effect, clouds, etc.
	class scenery final
	{
	public:
		static constexpr size_t size = 8;

	private:
		std::array<std::shared_ptr<gdk::entity>, size> m_ParallaxEntities;
		std::array<std::shared_ptr<gdk::material>, size> m_ParallaxMaterials;

		float time = 0;

	public:
		void update(const float delta);

		scenery(gdk::graphics::context::context_shared_ptr_type pContext,
			gdk::graphics::context::shader_program_shared_ptr_type pShader,
			gdk::graphics::context::scene_shared_ptr_type pScene,
			flappy::assets::shared_ptr aAssets);
		~scenery() = default;
	};
};

#endif
