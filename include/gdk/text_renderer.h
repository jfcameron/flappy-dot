// © Joseph Cameron - All Rights Reserved

#ifndef GDK_TEXT_RENDERER_H
#define GDK_TEXT_RENDERER_H

#include <gdk/graphics/context.h>
#include <gdk/graphics/entity.h>
#include <gdk/graphics/model.h>
#include <gdk/graphics/scene.h>
#include <gdk/graphics/types.h>
#include <gdk/text_map.h>

#include <string>

namespace gdk
{
	/// \brief renders a string as a single entity, one quad per character, using the rasters in a text_map
	///
	/// \remark this is a bitmap font renderer. For TTF text see gdk::graphics::ext::text_modeler
	class text_renderer
	{
	public:
		/// \brief alignment of the rendered text in world space
		enum class alignment
		{
			left_edge, //!< origin is set to the center of the left edge
			right_edge, //!< origin is set to the center of the right edge
			upper_edge, //!< origin is set to the center of the upper edge
			lower_edge, //!< origin is set to the center of the bottom edge
			left_upper_corner, //!< origin is set to the upper left corner
			left_lower_corner, //!< origin is set to the bottom left corner
			right_upper_corner, //!< origin is set to the upper right corner
			right_lower_corner, //!< origin is set to the lower right corner
			center, //!< origin is set to the center of the render
		};

	private:
		/// \brief ptr to the user's graphics context
		graphics::context_ptr_type m_pContext;

	protected:
		text_map m_TextMap;

		alignment m_Alignment;

		graphics::material_ptr_type m_Material;

		graphics::model_ptr_type m_Model;

		graphics::entity_ptr_type m_Entity;

		//! builds model data for the string, uploads it to the model
		void build_string_model(const graphics::model::usage_hint aHint, const std::wstring &aText);

		/// \brief constructor
		text_renderer(graphics::context_ptr_type pContext,
			text_map aTextMap,
			const alignment aAlignment,
			graphics::material_ptr_type aMaterial = nullptr);

	public:
		/// \brief prevents the text from being rendered
		void hide();

		/// \brief marks the text for rendering
		void show();

		/// \brief whether or not the text will render within the scene
		bool is_hidden() const;

		/// \brief set postion, rotation, scale of the text
		void set_transform(const graphics::vector3_type &aWorldPos,
			const graphics::quaternion_type &aRotation = graphics::quaternion_type::identity,
			const graphics::vector3_type &aScale = graphics::vector3_type::one);

		/// \brief can be added to multiple scenes
		void add_to_scene(graphics::scene_ptr_type pScene);

		/// \brief removes the text renderer's entity from the scene
		/// if the scene does not contain it, no error will occur
		void remove_from_scene(graphics::scene_ptr_type pScene);

		virtual ~text_renderer() = default;
	};
}

#endif
