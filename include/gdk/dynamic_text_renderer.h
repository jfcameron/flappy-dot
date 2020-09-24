// © 2020 Joseph Cameron - All Rights Reserved

#ifndef GDK_DYNAMIC_TEXT_RENDER_H
#define GDK_DYNAMIC_TEXT_RENDER_H

#include <gdk/text_renderer.h>

namespace gdk
{
	/// \brief base text renderer
	class dynamic_text_renderer final : public text_renderer
	{
	public:
		void update_text(const std::wstring &string);

		dynamic_text_renderer(gdk::graphics::context::context_shared_ptr_type pContext,
			gdk::text_map aTextMap,
			std::wstring aText = L" ");

		~dynamic_text_renderer() = default;
	};
}

#endif
