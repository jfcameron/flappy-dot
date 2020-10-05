// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_FLAPPY_PIPE_H
#define JFC_FLAPPY_PIPE_H

#include <gdk/graphics_context.h>
#include <gdk/scene.h>
#include <gdk/entity.h>
#include <gdk/model.h>
#include <gdk/input_context.h>

#include <jfc/assets.h>

#include <random>
#include <array>

namespace flappy
{
	/// \brief the game's obstacles
	class pipe final
	{
	public:
		enum class set_up_model
		{
			up_pipe,
			down_pipe
		};

	private:
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


		gdk::Vector2<float> m_Position;
		gdk::Vector2<float> m_Scale;
		float m_Rotation = 0;

	public:
		decltype(m_Position) getPosition() const;
		
		decltype(m_Scale) getScale() const;

		decltype(m_Rotation) getRotation() const;

		void update(const float delta, gdk::input::context::context_shared_ptr_type pInput);

		bool check_collision(const gdk::graphics_mat4x4_type &aWorldPosition) const;

		void set_up(const decltype(m_Position)& aPosition, const decltype(m_Rotation) aRotation, const set_up_model &aModel);

		pipe(gdk::graphics::context::context_shared_ptr_type pContext,
			gdk::graphics::context::scene_shared_ptr_type pScene,
			flappy::assets::shared_ptr aAssets);

		~pipe() = default;
	};
};

#endif
