// © 2020 Joseph Cameron - All Rights Reserved

#ifndef GDK_DYNAMIC_TEXT_RENDER_H
#define GDK_DYNAMIC_TEXT_RENDER_H

#include <gdk/graphics_context.h>
#include <gdk/scene.h>
#include <gdk/entity.h>

#include <random>
#include <array>

#include <gdk/input_context.h>
#include <gdk/text_map.h>

namespace gdk
{
	/// \brief renders text that can be updated
	class dynamic_text_renderer final
	{

	public:
		void update_text(const std::string &string);

		//dynamic_text_renderer();
	};
}

#endif
