// © 2020 Joseph Cameron - All Rights Reserved

#ifndef GDK_STATIC_TEXT_RENDER_H
#define GDK_STATIC_TEXT_RENDER_H

#include <gdk/text_renderer.h>

namespace gdk
{
	/// \brief renders text that cannot be updated
	class static_text_renderer final : public text_renderer
	{
	public:
		static_text_renderer(gdk::graphics::context::context_shared_ptr_type pContext,
			gdk::text_map aTextMap,
			std::wstring aText);

		~static_text_renderer() = default;
	};
}

#endif
