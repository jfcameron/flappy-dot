// © 2020 Joseph Cameron - All Rights Reserved

#ifndef GDK_STATIC_TEXT_RENDER_H
#define GDK_STATIC_TEXT_RENDER_H

#include <gdk/graphics_context.h>
#include <gdk/scene.h>
#include <gdk/entity.h>

#include <random>
#include <array>

#include <gdk/input_context.h>
#include <gdk/text_map.h>

using vertex_attribute_type = float;
using vertex_attribute_array_type = std::vector<vertex_attribute_type>;

namespace gdk
{
	/// \brief renders text that cannot be updated
	/// TODO: should be a kind of entity
	/// possibly entity should be split into an interface? not sure.
	/// this needs to be done so that the user can freely attach the text render to whateverscene etc. 
	/// hide it etc. without code dupes
	//
	// this is clarifying my earlier naval gazing about unicode rendering.
	// expand this to support multiple models, expand this to map to many textures
	// in this way, chinese characters can be spread across a number of textures (to fit platform texture size limitations)
	//
	// shader program with maximum # of textures bound,
	// write texture index into vertex data
	//
	// Width rules? support for carriage returns?
	// width rule in local space. yes thats a good one
	//
	// multi directional support, so render down, left, etc.
	//
	// add character index to vertex data for cool effects
	//
	// text direction, new line direction.
	//
	// individual character spacing (kerning) with a default value of {1} (full width)
	//
	// line spacing (space between lines)
	//
	// Text anchoring
	//
	class static_text_renderer final
	{
		using entity_ptr_type = std::shared_ptr<gdk::entity>;
		using material_ptr_type = std::shared_ptr<gdk::material>;

		entity_ptr_type m_Entity;
		material_ptr_type m_Material;
		
		gdk::text_map m_TextMap;

		// replace this crap with a 4x4
		gdk::Vector3<float> m_Position;
		gdk::Vector3<float> m_Scale = gdk::Vector3<float>::One;
		gdk::Quaternion<float> m_Rotation;

		void build_character_quad(vertex_attribute_array_type& posData, 
			vertex_attribute_array_type& uvData, 
			const char character,
			gdk::IntVector2<size_t> aCharacterOffsetInCells);

	public:
		//! can be added to multiple scenes
		void add_to_scene(gdk::graphics::context::scene_shared_ptr_type pScene);

		static_text_renderer(std::string aText,
			gdk::text_map aTextMap,
			gdk::graphics::context::context_shared_ptr_type pContext
		);
	};
}

#endif
